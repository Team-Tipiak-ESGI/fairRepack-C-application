//
// Objective of this application is extracting daily an XML file using fair-repack stock API,
// Then Send it to another application, which is located on the master server. It is done like that because we
// need to merge the xml files we generated.
// TL;DR : need to use CURL for API and FTP
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>

#include "Configuration.h"
#include "ftp.h"
#define  FOLDER "/home/erwan"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);
    char * str = malloc(10);
    char * filepath;

    FILE * file;
    Item * config = Configuration__loadFromFile("extractor.conf");

    char * warehouse_name = Item__getByKey(config, "warehouse_name")->value;
    char * warehouse_id = Item__getByKey(config, "warehouse_id")->value;
    char * filename = malloc(sizeof(warehouse_id) + sizeof(warehouse_name) + 5);

    // Build file name
    strcpy(filename, warehouse_id);
    strcat(filename, warehouse_name);

    sprintf(str, "%02d", local->tm_mon + 1);
    strcat(filename, str);

    sprintf(str, "%d", local->tm_mday);
    strcat(filename, str);

    filepath = malloc(sizeof(FOLDER) + sizeof(filename) + 6);
    strcpy(filepath, FOLDER);
    strcat(filepath, "/");
    strcat(filepath, filename);
    strcat(filepath, ".xml");

    file = fopen(filepath, "w+");

    free(str);
    free(filename);
    fputs(ptr, file);

    fclose(file);

    *((char **)userdata) = filepath;
    return size * nmemb;
}


int main(void){
    // Load from Configuration File
    Item * config = Configuration__loadFromFile("extractor.conf");

    char * api_url = Item__getByKey(config, "url")->value;
    char * warehouse_id = Item__getByKey(config, "warehouse_id")->value;
    char * url = malloc(sizeof("https://") + sizeof(api_url) + sizeof("/api/stock/read.php?warehouse=") + sizeof(warehouse_id));

    strcpy(url, "https://");
    strcat(url, api_url);
    strcat(url, "/api/stock/read.php?warehouse=");
    strcat(url, warehouse_id);

    CURL *curl;
    CURLcode result;

    char * filepath;

    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &filepath);

        result = curl_easy_perform(curl);
        if (result != CURLE_OK)
            fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(result));

        printf("%s\n", filepath);
        upload(filepath, curl);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "curl failed to init.\n");
        return EXIT_FAILURE;
    }


    curl_global_cleanup();
    free(filepath);
    free(url);
    return EXIT_SUCCESS;
}