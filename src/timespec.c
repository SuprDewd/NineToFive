#include "timespec.h"
#include "dateutil.h"
#include <stdio.h>

#define TIMESPEC_HOUR 0
#define TIMESPEC_MIN 1
#define TIMESPEC_SEC 2
#define TIMESPEC_NONE 3

time_t resolve_timespec(time_t relative_to, const char *tspec) {

    /* TODO: try parsing as absolute date/time string */

    const char *at = tspec;

    /* leading whitespace */
    while (*at && *at == ' ')
        at++;

    /* identity timespec */
    if (!*at)
        return relative_to;

    /* check relativity */
    int mul = 0;
    if (*at == '+')
        mul = 1, at++;
    else if (*at == '-')
        mul = -1, at++;

    int parts[3] = {0,0,0},
        pat = 0,
        suffix = TIMESPEC_NONE;

    /* simple state machine for parsing the spec */
    while (*at) {
        if ('0' <= *at && *at <= '9') {
            parts[pat] = parts[pat] * 10 + (*at - '0');
            at++;
        } else if (*at == ':') {
            pat++;
            if (pat >= 3) {
                return TIMESPEC_INVALID;
            }

            at++;
        } else if (*at == 's') {
            if (suffix != TIMESPEC_NONE) {
                return TIMESPEC_INVALID;
            }

            suffix = TIMESPEC_SEC;
            at++;
        } else if (*at == 'm') {
            if (suffix != TIMESPEC_NONE) {
                return TIMESPEC_INVALID;
            }

            suffix = TIMESPEC_MIN;
            at++;
        } else if (*at == 'h') {
            if (suffix != TIMESPEC_NONE) {
                return TIMESPEC_INVALID;
            }

            suffix = TIMESPEC_HOUR;
            at++;
        } else {
            return TIMESPEC_INVALID;
        }
    }

    /* if dealing with an interval */
    if (suffix == TIMESPEC_SEC ||
        suffix == TIMESPEC_MIN ||
        suffix == TIMESPEC_HOUR) {

        if (mul == 0) {
            return TIMESPEC_INVALID;
        }

        /* make sure precision is not sub-second */
        if (pat + 1 > 3 - suffix) {
            return TIMESPEC_INVALID;
        }

        /* move everything to correct place, so we have {H, M, S} in parts */
        int i;
        for (i = 2; i - suffix >= 0; i--) {
            parts[i] = parts[i - suffix];
        }

        /* clear trash */
        for ( ; i >= 0; i--) {
            parts[i] = 0;
        }

    } else if (suffix == TIMESPEC_NONE) {
        /* if dealing with regular time */

        /* make sure everything is in range */
        if (!(0 <= parts[0] && parts[0] < 24 &&
              0 <= parts[1] && parts[1] < 60 &&
              0 <= parts[2] && parts[2] < 60)) {
            return TIMESPEC_INVALID;
        }
    }

    struct tm rel;
    gmtime_r(&relative_to, &rel);

    if (suffix == TIMESPEC_SEC ||
        suffix == TIMESPEC_MIN ||
        suffix == TIMESPEC_HOUR) {
        rel.tm_hour += mul * parts[0];
        rel.tm_min += mul * parts[1];
        rel.tm_sec += mul * parts[2];
        return xtimegm(&rel);

    } else if (suffix == TIMESPEC_NONE) {
        if (mul == 1) {
            if (rel.tm_hour > parts[0] ||
                (rel.tm_hour == parts[0] && rel.tm_min > parts[1]) ||
                (rel.tm_hour == parts[0] && rel.tm_min == parts[1] && rel.tm_sec > parts[2])) {
                rel.tm_mday++;
            }
        } else if (mul == -1) {
            if (rel.tm_hour < parts[0] ||
                (rel.tm_hour == parts[0] && rel.tm_min < parts[1]) ||
                (rel.tm_hour == parts[0] && rel.tm_min == parts[1] && rel.tm_sec < parts[2])) {
                rel.tm_mday--;
            }
        }

        rel.tm_hour = parts[0];
        rel.tm_min = parts[1];
        rel.tm_sec = parts[2];
        return xtimegm(&rel);
    }

    return TIMESPEC_INVALID;
}

