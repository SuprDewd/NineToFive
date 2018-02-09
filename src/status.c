#include "status.h"
#include "logging.h"
#include "util.h"
#include "dateutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char linebuf[2048];

struct status* get_log_status(char log_name[256]) {

    ensure_log_file_exists(log_name);
    FILE *log = open_log_file(log_name, "r");

    // TODO: don't assume that the log file is sorted by time, we should
    // explicitly sort before processing

    int last = 0, sum = 0, total_sum = 0, working = 0;
    time_t last_invoice = 0;
    while (fgets(linebuf, sizeof(linebuf), log)) {
        time_t at = from_utc_str(linebuf);

        // TODO: make parsing more robust
        if (strstr(linebuf, "start")) {
            last = at;
            working = 1;
        } else if (strstr(linebuf, "stop")) {
            sum += at - last;
            total_sum += at - last;
            last = at;
            working = 0;
        } else if (strstr(linebuf, "invoice")) {
	    sum = 0;
            last_invoice = at;
	    last = at;
        } else {
            fprintf(stderr, "note: unable to parse '%s'\n", linebuf);
        }
    }

    fclose(log);

    if (working) {
	    sum += get_now() - last;
	    total_sum += get_now() - last;
    }

    struct status* res = (struct status*)malloc(sizeof(*res));
    res->total_seconds = total_sum;
    res->current_seconds = sum;
    res->working = working;
    res->last_invoice = last_invoice;

    return res;
}
