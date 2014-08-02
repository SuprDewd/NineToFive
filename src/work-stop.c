#include "work-stop.h"

#include <stdio.h>

int sub_stop(struct global_options g, int argc, char *argv[]) {
    printf("inside stop\n");
    printf("using log %s\n", g.log);
    return 0;
}

