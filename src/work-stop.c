#include "work-stop.h"
#include "work.h"
#include "util.h"
#include "logging.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

static struct option long_options[] = {
    /* Global options */
    WORK_GLOBAL_OPTIONS

    /* Subcommand-specific options */

    {0, 0, 0, 0}
};

int sub_stop(struct global_options *g, int argc, char *argv[]) {

    /* Parse command-line arguments */
    while (1) {
        int option_index;
        int c = getopt_long(argc, argv, "-" WORK_GLOBAL_OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            WORK_PROCESS_GLOBAL_OPTIONS(c)
            case 1:
                /* We found a bare argument, must be the time of stop */
                printf("starting with argument %s\n", optarg);
                break;
            case '?':
                exit(1);
            default:
                fatal("got invalid character code %d while parsing arguments", c);
        }
    }

    log_event(g->log_name, "stop");

    return 0;
}

