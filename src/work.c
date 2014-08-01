#include <time.h>
#include <stdio.h>

void get_now_utc_str(char *out) {
    time_t now_epoch;
    struct tm now_utc;
    time(&now_epoch);
    gmtime_r(&now_epoch, &now_utc);
    asctime_r(&now_utc, out);
}

int main() {
    char now_utc_str[64];
    get_now_utc_str(&now_utc_str[0]);

    printf("%s\n", now_utc_str);

    return 0;
}
