//
// Objective : Merge files that are in a
//
//
//
//

#include <stdio.h> // fputc(), frpintf()
#include <stdlib.h> // itoa()
#include <string.h> // strcat()
#include <dirent.h> // opendir(), readdir()
#include <time.h>

#define MERGE_DIR "DIR_LOCATION"
#define MERGED_DIR "MERGED_DIR"

const char * dateStamp();
int merger();

int main(void){

    merger();

    char * filepath = malloc(sizeof(MERGE_DIR) + sizeof("/mergeFile"));

    strcpy(filepath, MERGE_DIR);
    strcat(filepath, "/mergeFile");

    rename(filepath, dateStamp());
    return EXIT_SUCCESS;
}

int merger(){

    DIR *d = opendir(MERGE_DIR);
    struct dirent *dir;
    char pointer;
    FILE *mergedFile = fopen("mergeFile", "a"); // append to the file + create if does not exists
    FILE *toMergeFile;

    // Check if Files will open without resulting in a crash
    if (mergedFile == NULL){ // file that get
        fprintf(stderr," Error, unable to open the file, failure at line %d, exiting...", __LINE__);
        exit(EXIT_FAILURE);
    } else {
        if (d) { // if directory to scan exists
            while ((dir = readdir(d)) != NULL){
                if (dir->d_type == DT_REG) { /* If the entry is a regular file */
                    char * toMergeFileLocation = malloc(sizeof(MERGE_DIR) + sizeof(dir->d_name) + 1);

                    strcpy(toMergeFileLocation, MERGE_DIR);
                    strcat(toMergeFileLocation, "/");
                    strcat(toMergeFileLocation, dir->d_name);

                    toMergeFile = fopen(toMergeFileLocation, "r");
                    if (toMergeFile == NULL) { // file that give
                        fprintf(stderr, " Error, unable to open the file, failure at line %d, exiting...", __LINE__);
                        exit(EXIT_FAILURE);
                    } else {
                        fputc('\n', mergedFile);
                        while ((pointer = fgetc(toMergeFile)) != EOF){
                            fputc(pointer,mergedFile);
                        }
                    }

                    free(toMergeFileLocation);
                    fclose(toMergeFile);
                }
            }
        } else {
            fprintf(stderr,"Error, unable to open the directory. Exiting.");
            exit(EXIT_FAILURE);
        }
    }
    fclose(mergedFile);
    return EXIT_SUCCESS;
}

const char * dateStamp(){

    time_t now;
    time(&now);

    struct tm *local = localtime(&now);
    char * dateStamp = malloc(10);

    char * str = malloc(10);
    sprintf(str, "%d", local->tm_mon);

    strcat(dateStamp, str);

    sprintf(str, "%d", local->tm_year);
    strcat(dateStamp, str);

    free(str);

    return dateStamp; //i.e "0421"
}
