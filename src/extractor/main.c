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

#define URL "https://pa.quozul.dev/api/stock/read.php?warehouse=1"

int main(void){

    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, URL);
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