#include "dateutil.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

char *get_now_utc_str(void) {

    time_t now_epoch;
    struct tm now_utc;
    time(&now_epoch);
    gmtime_r(&now_epoch, &now_utc);


    char *out = malloc((64 + 1) * sizeof(char));

    /* http://en.wikipedia.org/wiki/ISO_8601 */
    strftime(out, 64, "%Y-%m-%dT%H:%M:%SZ", &now_utc);

    return out;
}



