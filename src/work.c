#include "../config.h"
#include "work.h"
#include "work-start.h"
#include "work-stop.h"
#include "work-status.h"
#include "work-working.h"
#include "work-invoice.h"
#include "work-help.h"
#include "util.h"
#include "logging.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

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
    puts("\tstatus\t\tGet current status");
    puts("\tworking\t\tCheck if currently working");
    puts("\tinvoice\t\tMake an invoice");
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

            if (xstrncpy(global_options.log_name, optarg, sizeof(global_options.log_name)) == -1) {
                fatal("log name too long: '%s'", optarg);
            }

            if (!valid_log_name(global_options.log_name)) {
                fatal("log name invalid");
            }

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
    int (*init)(struct global_options*, int, char*[]);
} subcommands[] = {
    {"start",   sub_start},
    {"stop",    sub_stop},
    {"status",  sub_status},
    {"working", sub_working},
    {"invoice", sub_invoice},
    {"help",    sub_help},
    {0, 0}
};

struct index_list {
	int index;
	struct index_list *next;
};

static void similar_commands(const char *name) {
	struct index_list *best = NULL,
	                  *tail = NULL;
	int best_dist = -1,
	    namelen = strlen(name),
	    maxlen = 0,
	    t, i, j;
	for (t = 0; subcommands[t].name; t++) {
		maxlen = MAX(maxlen, strlen(subcommands[t].name));
	}

	int **dist = (int**)calloc(namelen+1, sizeof(int*));
	for (i = 0; i <= namelen; i++) {
		dist[i] = (int*)calloc(maxlen+1, sizeof(int));
	}

	for (t = 0; subcommands[t].name; t++) {
		int curlen = strlen(subcommands[t].name);
		for (i = 0; i <= namelen; i++) {
			for (j = 0; j <= curlen; j++) {
				if (i == 0 || j == 0) {
					dist[i][j] = MAX(i,j);
				} else {
					dist[i][j] = (name[i-1] == subcommands[t].name[j-1] ? 0 : 1) + dist[i-1][j-1];
					dist[i][j] = MIN(dist[i][j], 1 + dist[i][j-1]);
					dist[i][j] = MIN(dist[i][j], 1 + dist[i-1][j]);
				}
			}
		}

		int cur_dist = dist[namelen][curlen];
		if (cur_dist < best_dist || best_dist == -1) {
			best_dist = cur_dist;
			while (best) {
				tail = best;
				best = best->next;
				free(tail);
			}
			tail = NULL;
		}
		if (cur_dist == best_dist) {
			struct index_list* add = (struct index_list*)malloc(sizeof(struct index_list));
			add->index = t;
			add->next = NULL;

			if (tail) {
				tail->next = add;
			} else {
				best = add;
			}
			tail = add;
		}
	}

	for (i = 0; i <= namelen; i++) free(dist[i]);
	free(dist);

	fprintf(stderr, "\nThe most similar commands are\n");
	while (best) {
		fprintf(stderr, "        %s\n", subcommands[best->index].name);
		tail = best;
		best = best->next;
		free(tail);
	}
}

static int dispatch(const char *name, int argc, char *argv[]) {
    int i;
    for (i = 0; subcommands[i].name; i++) {
        if (strcmp(name, subcommands[i].name) == 0) {
            return subcommands[i].init(&global_options, argc, argv);
        }
    }

    fprintf(stderr, "'%s' is not a valid subcommand\n", name);
    similar_commands(name);
    exit(1);
}

void main(int argc, char *argv[]) {

    /* Default options */
    strcpy(global_options.log_name, "main");

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
