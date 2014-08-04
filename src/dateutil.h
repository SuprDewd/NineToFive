#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <time.h>

char *to_utc_str(time_t t);
time_t get_now(void);
time_t xtimegm(struct tm *tm);

#endif
