#include "datestamp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char * dateStamp(char bool){
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    char * dateStamp = malloc(10);
    char * str = malloc(10);
    sprintf(str, "%d", local->tm_mon);
    strcat(dateStamp, str);
    if(bool == 1) {
        sprintf(str, "%d", local->tm_year);
        strcat(dateStamp, str);
    }
    free(str);
    return dateStamp; //i.e "0421"
}
