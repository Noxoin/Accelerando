#include "server_api.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <curl/curl.h>

#define BUF_SIZE 64
#define LOGIN_URL "http://accelerando.noxoin.com/api/v1/login/"
#define LOGIN_BODY "username=%s&password=%s"
#define SCORE_URL "http://accelerando.noxoin.com/api/v1/song/"
#define SCORE_BODY "username=%s&token=%s&song_id=%d&score=%d&accuracy=%.2f"

struct bufdata {
    char *token;
    int len;
};

void abort_(const char * s, ...) {
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

static size_t write_cb(void * ptr, size_t size, size_t nmemb, void *usrptr) {
    struct bufdata * bd = (struct bufdata *) usrptr;
    if(size*nmemb > BUF_SIZE) {
        return 0;
    }
    memcpy(bd->token, (char *) ptr, size*nmemb);
    return size*nmemb;
}

User::User(char * user, char * password) {
    token = (char *) malloc(sizeof(char)*BUF_SIZE+4*5);
    char * bd_token = (char *) malloc(sizeof(char)*BUF_SIZE+4*5);
    username = (char *) malloc(sizeof(char)*strlen(user)+4*5);
    memcpy(username, (char *) user, strlen(user));

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(!curl) {
        abort_("[User] could not initialize curl\n");
    }
    char * url = (char *) malloc(sizeof(char)*strlen(LOGIN_URL)+4*5);
    char * post_body = (char *) malloc(sizeof(char)*(19+strlen(username)+strlen(password))+4*5);
    struct bufdata bd;
    bd.token = bd_token;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bd);

    sprintf(url, LOGIN_URL);
    sprintf(post_body, LOGIN_BODY, username, password);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_body);

    bd.len = 0;
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        abort_("[User] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    memcpy(token, bd_token, strlen(bd_token));

    free(url);
    free(post_body);
    curl_easy_cleanup(curl);
}

User::~User() {
}

bool User::isLoggedIn(){
    if(strlen(token) > 0 ) {
        return true;
    } else {
        return false;
    }
}

void User::recordScore(int song_id, int score, float accuracy) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(!curl) {
        abort_("[User] could not initialize curl\n");
    }
    char * url = (char *) malloc(sizeof(char)*strlen(SCORE_URL)+4*5);
    char * post_body = (char *) malloc(sizeof(char)*(42+strlen(username)+strlen(token)+15+6)+4*5);
    struct bufdata bd;
    bd.token = token;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bd);

    sprintf(url, SCORE_URL);
    sprintf(post_body, SCORE_BODY, username, token, song_id, score, accuracy);
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_body);

    bd.len = 0;
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        abort_("[User] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    free(url);
    free(post_body);
    curl_easy_cleanup(curl);
}

