#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "Location.h"

#define REMOTE_PATH "sftp://fairrepack.sagliss.industries/ftp/toMergeFile"
#define USERNAME "ftpuser"
#define PASSWORD "fairrepackFTPAgent2021"

struct WriteThis {
    char * readptr;
    size_t sizeleft;
};

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userp)
{
    struct WriteThis *upload = (struct WriteThis *)userp;
    size_t max = size*nmemb;

    if(max < 1)
        return 0;

    if(upload->sizeleft) {
        size_t copylen = max;
        if(copylen > upload->sizeleft)
            copylen = upload->sizeleft;
        memcpy(ptr, upload->readptr, copylen);
        upload->readptr += copylen;
        upload->sizeleft -= copylen;
        return copylen;
    }

    return 0;                          /* no more data left to deliver */
}


int upload(struct Location *location, char *filename, CURL *curl)
{
    //CURL *curl;
    CURLcode res;

    FILE * file = fopen(location->filepath, "r");

    struct WriteThis upload;
    char * buffer;

    // jump to end of file
    fseek(file, 0, SEEK_END);
    // get offset (used for length)
    upload.sizeleft = ftell(file);
    // go back to start of file
    rewind(file);

    // allocate memory for the buffer
    buffer = (char*)malloc(upload.sizeleft * sizeof(char));
    // write file data to buffer
    fread(buffer, 1, upload.sizeleft, file);

    //strcpy(upload.readptr, buffer);
    upload.readptr = buffer;


    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        char * url = malloc(sizeof(REMOTE_PATH) + strlen(location->filename) + 1);

        // sftp://fairrepack.sagliss.industries/ftp/toMergeFile/file.txt
        strcpy(url, REMOTE_PATH);
        strcat(url, "/");
        strcat(url, location->filename);
        printf("%s", url);

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload);
        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}