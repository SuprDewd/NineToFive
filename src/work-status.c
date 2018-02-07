#include "work-status.h"
#include "work.h"
#include "util.h"
#include "dateutil.h"
#include "logging.h"
#include "timespec.h"
#include "status.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

static struct option long_options[] = {
    /* Global options */
    WORK_GLOBAL_OPTIONS

    /* Subcommand-specific options */

    {0, 0, 0, 0}
};

int sub_status(struct global_options *g, int argc, char *argv[]) {

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

    struct status *st = get_log_status(g->log_name);

    printf("current status: ");
    if (st->working) {
        printf("working\n");
    } else {
        printf("not working\n");
    }

    if (st->last_invoice != 0) {
	    char *utc_str = to_utc_str(st->last_invoice);
	    printf("last invoice: %s\n", utc_str);
	    free(utc_str);
    }

    int s = st->total_seconds;
    printf("uninvoiced: %02d:%02d:%02d\n", s / 60 / 60, (s / 60) % 60, s % 60);

    free(st);
    return 0;
}

