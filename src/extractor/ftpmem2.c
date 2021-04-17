/* <DESC>
 * FTP upload a file from memory
 * </DESC>
 */
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define FTP_URL "sftp://213.32.6.40/ftp/test.txt"

static const char data[]=
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Nam rhoncus odio id venenatis volutpat. Vestibulum dapibus "
        "bibendum ullamcorper. Maecenas finibus elit augue, vel "
        "condimentum odio maximus nec. In hac habitasse platea dictumst. "
        "Vestibulum vel dolor et turpis rutrum finibus ac at nulla. "
        "Vivamus nec neque ac elit blandit pretium vitae maximus ipsum. "
        "Quisque sodales magna vel erat auctor, sed pellentesque nisi "
        "rhoncus. Donec vehicula maximus pretium. Aliquam eu tincidunt "
        "lorem.";

struct WriteThis {
    const char *readptr;
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

int main(void)
{
    CURL *curl;
    CURLcode res;

    struct WriteThis upload;

    upload.readptr = data;
    upload.sizeleft = strlen(data);

    /* In windows, this will init the winsock stuff */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    /* Check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n",
                curl_easy_strerror(res));
        return 1;
    }

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL, the target file */
        fprintf(stdout, "%s", FTP_URL);
        curl_easy_setopt(curl, CURLOPT_URL, FTP_URL);

        /* Now specify we want to UPLOAD data */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload);

        /* get verbose debug output please */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Set the expected upload size. */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)upload.sizeleft);

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