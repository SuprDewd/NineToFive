#include "work-start.h"
#include "work.h"
#include "util.h"
#include "dateutil.h"
#include "logging.h"
#include "timespec.h"
#include "status.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

static struct option long_options[] = {
    /* Global options */
    WORK_GLOBAL_OPTIONS

    /* Subcommand-specific options */

    {0, 0, 0, 0}
};

static char tspec[64];

int sub_start(struct global_options *g, int argc, char *argv[]) {

    int found_tspec = 0;

    /* Parse command-line arguments */
    while (1) {

        /* TODO: check for -0..-9, they should all be treated as tspec, not options */

        int option_index;
        int c = getopt_long(argc, argv, "-" WORK_GLOBAL_OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            WORK_PROCESS_GLOBAL_OPTIONS(c)
            case 1:
                /* We found a bare argument, must be the time of start */

                if (found_tspec) {
                    fatal("too many parameters to start");
                }

                if (xstrncpy(tspec, optarg, sizeof(tspec)) < 0) {
                    fatal("parameter to start is too long");
                }

                found_tspec = 1;

                break;
            case '?':
                exit(1);
            default:
                fatal("got invalid character code %d while parsing arguments", c);
        }
    }

    time_t time;

    if (found_tspec) {
        time = resolve_timespec(get_now(), tspec);
        if (time == TIMESPEC_INVALID) {
            fatal("time specification '%s' is invalid", tspec);
        }
    } else {
        time = get_now();
    }

    struct status *st = get_log_status(g->log_name);
    if (st->working) fatal("current status is 'working'");
    free(st);

    log_event(g->log_name, time, "start");

    return 0;
}

