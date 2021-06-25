#include "datestamp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//get date timestamp
const char * dateStamp(void){
    time_t now;
    time(&now);

    char * dateStamp = malloc(10);
    struct tm *local = localtime(&now);
    char * str = malloc(10);

    sprintf(str, "20%02d", local->tm_year % 100);
    strcat(dateStamp, str);

    free(str);

    return dateStamp;
}

// get months timestamp
const char * monthStamp(void){
    time_t now;
    time(&now);

    char * monthStamp = malloc(10);
    struct tm *local = localtime(&now);
    char * str = malloc(10);

    sprintf(str, "%02d", local->tm_mon + 1);
    strcat(monthStamp, str);

    free(str);

    return monthStamp;
}

