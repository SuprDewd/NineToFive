#ifndef LOGGING_H
#define LOGGING_H

#include <time.h>
#include <stdio.h>

void ensure_log_dir_exists(void);
void ensure_log_file_exists(const char *log_name);
char *get_log_file(const char *log_name);
int valid_log_name(const char *log_name);
FILE *open_log_file(const char *log_name, const char *mode);
void log_event(const char *log_name, time_t time, const char *event);
void log_event_now(const char *log_name, const char *event);

#endif
