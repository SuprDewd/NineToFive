#include "work-status.h"
#include "work.h"
#include "util.h"
#include "dateutil.h"
#include "logging.h"
#include "timespec.h"

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

static char linebuf[2048];

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


    // TODO: extract to a separate parsing routine
    ensure_log_file_exists(g->log_name);
    FILE *log = open_log_file(g->log_name, "r");

    // TODO: don't assume that the log file is sorted by time, we should
    // explicitly sort before processing

    int last = 0, sum = 0, working = 0;
    while (fgets(linebuf, sizeof(linebuf), log)) {
        time_t at = from_utc_str(linebuf);

        // TODO: make parsing more robust
        if (strstr(linebuf, "start")) {
            last = at;
            working = 1;
        } else if (strstr(linebuf, "stop")) {
            sum += at - last;
            last = at;
            working = 0;
        } else {
            fprintf(stderr, "note: unable to parse '%s'\n", linebuf);
        }
    }

    printf("current status: ");
    if (working) {
        printf("working\n");
    } else {
        printf("not working\n");
    }

    printf("hours so far: ");

    printf("%02d:%02d:%02d\n", sum / 60 / 60, (sum / 60) % 60, sum % 60);

    if (working)

    return 0;
}

