#ifndef STATUS_H
#define STATUS_H

struct status {
	int total_seconds; // Total number of seconds of work
	int working;       // Whether work is currently in progress
};

struct status* get_log_status(char log_name[256]);

#endif
