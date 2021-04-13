//
// Objective : Merge files that are in a
//
//
//
//

#include <dos.h> // getdate()
#include <stdio.h> // fputc(), frpintf()
#include <stdlib.h> // itoa()
#include <string.h> // strcat()
#include <dirent.h> // opendir(), readdir()

char dateStamp();
void merger();

int main(int argc, char *argv[]){

    return EXIT_SUCCESS;
}

int merger(char *toMergeFileLocation){

    DIR *d = opendir(path);
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
                    *toMergeFile = fopen(toMergeFileLocation, "r");
                    if (toMergeFile == NULL) { // file that give
                        fprintf(stderr, " Error, unable to open the file, failure at line %d, exiting...", __LINE__);
                        exit(EXIT_FAILURE);
                    } else {
                        while ((pointer = fgetc(toMergeFile)) != EOF){
                            fputc(ch,mergedFile);
                        }
                    }
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

char dateStamp(){
    struct date d;
    char *dateStamp;
    getdate(&d);
    strcat(dateStamp,itoa(d.da_mon));
    strcat(dateStamp,itoa(d.da_year);
    return dateStamp;
}