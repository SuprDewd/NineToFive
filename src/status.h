#ifndef STATUS_H
#define STATUS_H

#include <time.h>

struct status {
	int total_seconds;   // Total number of seconds of work
	int current_seconds; // Number of seconds of work since last invoice
	int working;         // Whether work is currently in progress
	time_t last_invoice; // Time of last invoice
};

struct status* get_log_status(char log_name[256]);

#endif
