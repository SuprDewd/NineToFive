#ifndef TIMESPEC_H
#define TIMESPEC_H

#include <time.h>

#define TIMESPEC_INVALID ((time_t)-1)
time_t resolve_timespec(time_t relative_to, const char *tspec);

#endif
