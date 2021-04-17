//
// Objective : Merge files that are in a specific directory then the result are stored in another
//
//
//
//

#include <stdio.h> // fputc(), frpintf()
#include <stdlib.h> // itoa()
#include <string.h> // strcat()
#include <dirent.h> // opendir(), readdir()
#include <sys/stat.h> // file descriptor
#include <errno.h> // errno macro
#include "../common/datestamp.h"

#define MERGE_DIR "/var/www/fairrepack.sagliss.industries/ftp/toMergeFiles"
#define MERGED_DIR "/var/www/fairrepack.sagliss.industries/ftp/mergedFiles"

int merger(void);
void dirCreator(void);

int main(void){
    dirCreator();
    merger();

    char * filepath = malloc(sizeof(MERGE_DIR) + sizeof("/mergeFile"));

    strcpy(filepath, MERGE_DIR);
    strcat(filepath, "/mergeFile");

    rename(filepath, dateStamp(0));
    return EXIT_SUCCESS;
}

void dirCreator(void){

    const char *name = dateStamp(1);
    DIR *dir = opendir(MERGED_DIR);
    int dfd = dirfd(dir);
    errno = 0;
    int ret = mkdirat(dfd, name, S_IRWXU);
    if (ret == -1){
        switch (errno) {
            case EACCES:
                fprintf(stderr, "unallowed to write in the parent dir, exiting.");
                exit(EXIT_FAILURE);
            case EEXIST:
                fprintf(stderr,"warning: directory already exists.");
            case ENAMETOOLONG:
                fprintf(stderr,"directory has a too long name, exiting.");
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "mkdir failed, exiting.");
                exit(EXIT_FAILURE);
        }
    }
    closedir(dir);
}

int merger(void){

    DIR *d = opendir(MERGE_DIR);
    struct dirent *dir;
    char pointer;
    FILE *mergedFile = fopen("mergeFile", "a"); // append to the file + create if does not exists
    FILE *toMergeFile;

    // Check if Files will open without resulting in a crash
    if (mergedFile == NULL){ // file that get the file name
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

