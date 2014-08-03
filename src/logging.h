#ifndef LOGGING_H
#define LOGGING_H

void ensure_log_dir_exists(void);
void ensure_log_file_exists(const char *log_name);
char *get_log_file(const char *log_name);
int valid_log_name(const char *log_name);
void log_event(const char *log_name, const char *event);

#endif
