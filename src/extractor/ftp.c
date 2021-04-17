#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>


#define REMOTE_PATH "sftp://fairrepack.sagliss.industries/ftp/toMergeFile"
#define USERNAME "ftpuser"
#define PASSWORD "fairrepackFTPAgent2021"
struct WriteThis {
    const char *readptr;
    size_t sizeleft;
};


int upload(char *filePath, CURL *curl)
{
    //CURL *curl;
    CURLcode res;


    FILE * file = fopen(filePath, "r");

        /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        const char * filename = "file.txt";
        char * url = malloc(sizeof(REMOTE_PATH) + strlen(filename) + 1);

        strcpy(url, REMOTE_PATH);
        strcat(url, "/");
        strcat(url, filename);
        printf("%s", url);

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_READDATA, &file);
        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);

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