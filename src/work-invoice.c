#include "work-invoice.h"
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

int sub_invoice(struct global_options *g, int argc, char *argv[]) {

    int found_tspec = 0;

    /* Parse command-line arguments */
    while (1) {

        int option_index;
        int c = getopt_long(argc, argv, "-" WORK_GLOBAL_OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            WORK_PROCESS_GLOBAL_OPTIONS(c)
            case 1:
                /* We found a bare argument, but I'm not expecting one */
                fatal("unrecognized bare argument '%s' to status command", optarg);
                break;
            case '?':
                exit(1);
            default:
                fatal("got invalid character code %d while parsing arguments", c);
        }
    }

    time_t time = get_now();

    struct status *st = get_log_status(g->log_name);
    int working = st->working,
	s = st->total_seconds;
    free(st);

    if (working) log_event(g->log_name, time, "stop");

    printf("invoice: %02d:%02d:%02d\n", s / 60 / 60, (s / 60) % 60, s % 60);

    log_event(g->log_name, time, "invoice");
    if (working) log_event(g->log_name, time, "start");

    return 0;
}

