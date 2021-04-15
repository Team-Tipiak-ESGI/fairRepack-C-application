//
// Objective of this application is extracting daily an XML file using fair-repack stock API,
// Then Send it to another application, which is located on the master server. It is done like that because we
// need to merge the xml files we generated.
// TL;DR : need to use CURL for API and FTP
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>

#include "Configuration.h"


int main(void){
    // Load from Configuration File
    Item * config = Configuration__loadFromFile("extractor.conf");

    char * api_url = Item__getByKey(config, "url")->value;
    char * warehouse_id = Item__getByKey(config, "warehouse_id")->value;
    char *url = malloc(sizeof(api_url) + sizeof("?warehouse=") + sizeof(warehouse_id));

    strcpy(url, api_url);
    strcat(url, "?warehouse=");
    strcat(url, warehouse_id);

    printf("%s\n", Item__getByKey(config, "warehouse_name")->value);
    printf("%s\n", Item__getByKey(config, "warehouse_id")->value);
    printf("%s", url);

    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        result = curl_easy_perform(curl);
        if (result != CURLE_OK)
            fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "curl failed to init.\n");
        return EXIT_FAILURE;
    }

    curl_global_cleanup();
    return EXIT_SUCCESS;
}