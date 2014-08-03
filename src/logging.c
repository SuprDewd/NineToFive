#include "../config.h"
#include "logging.h"
#include "util.h"
#include "dateutil.h"

#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

static char *get_log_dir(void) {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    int log_dir_len = strlen(homedir) + 2 + strlen(PACKAGE_NAME);
    char *log_dir = malloc(sizeof(char) * (log_dir_len + 1));

    strcpy(log_dir, homedir);
    strcat(log_dir, "/.");
    strcat(log_dir, PACKAGE_NAME);

    return log_dir;
}

char *get_log_file(const char *log_name) {
    char *dir = get_log_dir();
    int dirlen = strlen(dir);

    int log_file_len = dirlen + 1 + strlen(log_name) + 4;
    char *log_file = malloc(sizeof(char) * (log_file_len + 1));

    strcpy(log_file, dir);
    strcat(log_file, "/");
    strcat(log_file, log_name);
    strcat(log_file, ".log");

    return log_file;
}

void ensure_log_dir_exists(void) {
    char *dir = get_log_dir();

    struct stat st;
    if (stat(dir, &st) == -1 && errno != ENOENT) {
        fatal("couldn't stat log directory: %s", strerror(errno));
    }

    if (errno == ENOENT) {
        /* Log directory doesn't exist, create it */

        if (mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1) {
            fatal("couldn't create log directory: %s", strerror(errno));
        }
    } else if (!S_ISDIR(st.st_mode)) {
        fatal("log directory path already exists, but isn't a directory");
    }

    free(dir);
}

void ensure_log_file_exists(const char *log_name) {
    ensure_log_dir_exists();

    char *log_file = get_log_file(log_name);

    struct stat st;
    if (stat(log_file, &st) == -1 && errno != ENOENT) {
        fatal("couldn't stat log file: %s", strerror(errno));
    }

    if (errno == ENOENT) {
        /* Log file doesn't exist, create it */
        if (creat(log_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
            fatal("couldn't create log file: %s", strerror(errno));
        }
    } else if (!S_ISREG(st.st_mode)) {
        fatal("log file path already exists, but isn't a regular file");
    }

    free(log_file);
}

int valid_log_name(const char *log_name) {
    int len = strlen(log_name);
    if (len == 0) {
        return 0;
    }

    int i;
    for (i = 0; i < len; i++) {
        if (log_name[i] == '/' || log_name[i] == '.') {
            return 0;
        }
    }

    return 1;
}

static FILE *open_log_file(const char *log_name, const char *mode) {
    char *path = get_log_file(log_name);
    FILE *f = fopen(path, mode);

    if (!f) {
        fatal("could not open log file: %s", strerror(errno));
    }

    free(path);
    return f;
}

void log_event(const char *log_name, const char *event) {
    ensure_log_file_exists(log_name);

    char *now = get_now_utc_str();
    FILE *log = open_log_file(log_name, "a");

    if (fprintf(log, "%s %s\n", now, event) < 0) {
        fatal("could not write to log file: %s", strerror(errno));
    }

    fclose(log);
}

