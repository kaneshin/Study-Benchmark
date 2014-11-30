#include <stdio.h>
#include <stdbool.h>

#include "leap.h"

bool is_leap(int year)
{
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }
    return false;
}

void output_leap()
{
    int i;
    for (i = 1; i <= 2014; i++) {
        if (is_leap(i)) {
            printf("%d\n", i);
        }
    }
}

