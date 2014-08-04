#include "dateutil.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

char *to_utc_str(time_t t) {
    struct tm utc;
    gmtime_r(&t, &utc);

    char *out = malloc((64 + 1) * sizeof(char));

    /* http://en.wikipedia.org/wiki/ISO_8601 */
    strftime(out, 64, "%Y-%m-%dT%H:%M:%SZ", &utc);

    return out;
}

time_t get_now(void) {
    time_t now;
    time(&now);
    return now;
}

/* http://linux.die.net/man/3/timegm */
time_t xtimegm(struct tm *tm) {
    time_t ret;
    char *tz;

    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz)
        setenv("TZ", tz, 1);
    else
        unsetenv("TZ");
    tzset();
    return ret;
}

