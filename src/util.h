#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

#define PBSTR "***************************************************************"
#define PBWIDTH 60

void print_progress(double ratio) {
    int val = (int)(ratio * 100);
    int lpad = (int)(ratio * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

#endif