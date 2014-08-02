#include "../config.h"
#include "work.h"
#include "work-start.h"
#include "work-stop.h"
#include "work-help.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

void get_now_utc_str(char *out) {
    time_t now_epoch;
    struct tm now_utc;
    time(&now_epoch);
    gmtime_r(&now_epoch, &now_utc);
    asctime_r(&now_utc, out);
}

static void version(void) {
    printf("%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

static void usage(void) {
    puts("Usage: work command [option]... [argument]...");
    puts("");
    puts("List of commands:");
    puts("\thelp\t\tShow help for a given command or a help overview");
    puts("\tstart\t\tLog a start event");
    puts("\tstop\t\tLog a stop event");
    puts("");
    puts("Global options:");
    puts("\t-h --help\t\tDisplay this help and exit");
    puts("\t-l --log LOG\t\tThe log to use");
    puts("\t-V --version\t\tOutput version information and exit");
}

static struct option long_options[] = {
    /* Global options */
    WORK_GLOBAL_OPTIONS

    {0, 0, 0, 0}
};

void process_global_options(int c) {
    switch (c) {
        case 'h':
            usage();
            exit(0);
        case 'l':
            if (xstrncpy(global_options.log, optarg, sizeof(global_options.log)) == -1)
                fatal("log name too long: '%s'", optarg);
            break;
        case 'V':
            version();
            exit(0);
        default:
            fatal("invalid argument encountered while processing global options");
    }
}

static struct {
    char *name;
    int (*init)(struct global_options, int, char*[]);
} subcommands[] = {
    {"start", sub_start},
    {"stop",  sub_stop},
    {"help",  sub_help},
    {0, 0}
};

static int dispatch(const char *name, int argc, char *argv[]) {
    int i;
    for (i = 0; subcommands[i].name; i++) {
        if (strcmp(name, subcommands[i].name) == 0) {
            return subcommands[i].init(global_options, argc, argv);
        }
    }

    fatal("'%s' is not a valid subcommand", name);
}

void main(int argc, char *argv[]) {

    /* Default options */
    strcpy(global_options.log, "main");

    /* Parse command line arguments */
    while (1) {
        int option_index;
        int c = getopt_long(argc, argv, "-" WORK_GLOBAL_OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            WORK_PROCESS_GLOBAL_OPTIONS(c)
            case 1:
                /* We found a bare argument, must be a sub-command */
                exit(dispatch(argv[optind-1], argc, argv));
            case '?':
                exit(1);
            default:
                fatal("got invalid character code %d while parsing arguments", c);
        }
    }

    usage();
    exit(1);
}
