#ifndef WORK_H
#define WORK_H

/* Global options */
struct global_options {
    char log[256];
} global_options;

#define WORK_GLOBAL_OPTIONS \
    {"help",    no_argument,       0, 'h'}, \
    {"log",     required_argument, 0, 'l'}, \
    {"version", no_argument,       0, 'V'}, \

#define WORK_GLOBAL_OPTSTR "hl:v"
#define WORK_PROCESS_GLOBAL_OPTIONS(c) \
    case 'h': \
    case 'l': \
    case 'V': \
        process_global_options(c); \
        break;

void process_global_options(int c);
/* End global options */

#endif
