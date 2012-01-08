/***************************************************************************
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 * Modified by Hadi Aladdin (support@coursepeer.com)
 * Downloaded from: http://www.github.com/hadialaddin/cpgraph
 *
 * Implementation for the C library for CoursePeer's Knowledge Graph API
 * using libcurl
 ***************************************************************************/ 

// YOU MUST MODIFY THESE DEFINITIONS TO SUIT YOUR APPLICATION
const char CP_APP_KEY[] = "mepeer";
const char CP_APP_SECRET[] = "mepeer";

// DO NOT MODIFY ANYTHING BELOW THIS LINE
const char CP_APP_REDIRECT_URI[] = "libcurl";

const char CP_AUTHORIZE_SERVER[] = "http://graph.coursepeer.com/index.php/oauth/";
const char CP_ACCESS_TOKEN_GRANT_SERVER[] = "http://graph.coursepeer.com/index.php/oauth/access_token/";
const char CP_METHOD_DATA_SERVER[] = "http://graph.coursepeer.com/index.php/data";

const char CP_REQUEST_CODE_URI_STARTER[] = "?response_type=code&scope=basic";
const char CP_REQUEST_CODE_URI_CLIENTID[] = "&client_id=";
const char CP_REQUEST_CODE_URI_REDIRECT_URI[] = "&redirect_uri=";
const char CP_REQUEST_CODE_URI_EMAIL[] = "&email=";
const char CP_REQUEST_CODE_URI_PASSWORD[] = "&password=";
const char CP_REQUEST_CODE_URI_DESKTOP[] = "&desktop=1";

const char CP_ACCESS_TOKEN_URI_STARTER[] = "?grant_type=authorization_code";
const char CP_ACCESS_TOKEN_URI_CLIENTID[] = "&client_id=";
const char CP_ACCESS_TOKEN_URI_CLIENTSECRET[] = "&client_secret=";
const char CP_ACCESS_TOKEN_URI_REDIRECT_URI[] = "&redirect_uri=";
const char CP_ACCESS_TOKEN_URI_CODE[] = "&code=";
const char CP_ACCESS_TOKEN_URI_DESKTOP[] = "&desktop=1";

const char CP_METHOD_DATA_ACCESS_TOKEN[] = "?access_token=";

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curl/curl.h"
#include <jansson.h>

// Memory Structure to hold the returned values from
// the HTTP cURL request
struct MemoryStruct {
	char *memory;
	size_t size;
};

// 
CURL *curl_handle; // cURL handler to do the connections


/* Function used as a callback for the cURL;
 * gets executed when it returns back from the call.
 */
static size_t write_memory_callback(const void *contents, size_t size, size_t nmemb, void *userp)
{
	const size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */ 
		fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}
 
/* 
 * Places the access_token value in the
 * passed return_access_token char array.
 * Returns 1 if failed, and 0 if succeeded.
 */
int request_access_token(char *returned_access_token, size_t returned_access_token_len, const char *email, const char *password, char *error, size_t error_len)
{
	// This function goes to the CoursePeer server, grabs a code
	// then returns back, to resend it and get an access_token.
	// Once access token is aquired, it stores it in 'token.txt'
	// file in the same directory of the executable, for future
	// retrieval.
	struct MemoryStruct code;
	struct MemoryStruct access_token_response;
	char cp_code_url[1000];
	char access_token_granting_url[400];

	json_t *access_token_json;
	json_error_t json_error;
	json_t *access_token;
	 
	code.memory = malloc(1);  /* will be grown as needed by the realloc above */
    assert(code.memory);
	code.size = 0;    /* no data at this point */ 

	access_token_response.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    assert(access_token_response.memory);
	access_token_response.size = 0;    /* no data at this point */ 
	 
	curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();

	snprintf(cp_code_url, sizeof(cp_code_url), "%s%s%s%s%s%s%s%s%s%s%s%s\n", CP_AUTHORIZE_SERVER, CP_REQUEST_CODE_URI_STARTER, CP_REQUEST_CODE_URI_CLIENTID,
        CP_APP_KEY, CP_REQUEST_CODE_URI_REDIRECT_URI, CP_APP_REDIRECT_URI, CP_REQUEST_CODE_URI_EMAIL, email, CP_REQUEST_CODE_URI_PASSWORD,
        password, CP_REQUEST_CODE_URI_DESKTOP);
	
    /* specify URL to get authorization code */
	curl_easy_setopt(curl_handle, CURLOPT_URL, cp_code_url);
	 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	 
	/* we pass our 'code' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&code);
	 
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	 
	/* get it! */ 
	curl_easy_perform(curl_handle);
	 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);

	if (strcmp("authentication_failed", code.memory) == 0) {
		// Authentication Failed
		snprintf(error, error_len, "Authentication Failure. The provided Email and Password are incorrect.");
		return 1; // Failed to authenticate user credentials
	}
	// Now, our code.memory points to a memory block that is code.size
	// bytes big and contains the CoursePeer Code. Next, is to exchange
	// this code for an access_token.

	snprintf(access_token_granting_url, sizeof(access_token_granting_url), "%s%s%s%s%s%s%s%s%s%s%s", CP_ACCESS_TOKEN_GRANT_SERVER, CP_ACCESS_TOKEN_URI_STARTER,
        CP_ACCESS_TOKEN_URI_CLIENTID, CP_APP_KEY, CP_ACCESS_TOKEN_URI_CLIENTSECRET, CP_APP_SECRET, CP_ACCESS_TOKEN_URI_REDIRECT_URI, CP_APP_REDIRECT_URI,
        CP_ACCESS_TOKEN_URI_CODE, code.memory, CP_ACCESS_TOKEN_URI_DESKTOP);

    curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();
	 
	/* specify URL to get */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, access_token_granting_url);
	 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	 
	/* we pass our 'code' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&access_token_response);
	 
	/* some servers don't like requests that are made without a user-agent
	 field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	 
	/* get it! */ 
	curl_easy_perform(curl_handle);
	 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);

	access_token_json = json_loads(access_token_response.memory, 0, &json_error);

	if(!access_token_json){
		// Something went wrong with the authorization server. Please try again later.
		snprintf(error, error_len, "Something went wrong with the authorization server. Please try again later.");
		return 1;
	}

	access_token = json_object_get(access_token_json, "access_token");

	// Keep memory clean before we return
    free(code.memory);
    free(access_token_response.memory);
 
	/* we're done with libcurl, so clean it up */ 
	curl_global_cleanup();

	// Make sure we have the access token and return, otherwise
	// return error code
	if (access_token && json_is_string(access_token)) {
		// Pointer is value until we do the next name refresh
		snprintf(returned_access_token, returned_access_token_len, json_string_value(access_token));
		return 0;
    }

    // Something went wrong, return error code
    snprintf(error, error_len, "Something went wrong. Try again later.");
    return 1;
}

int cp_api_method(char *result, size_t result_len, char *method, char *access_token, char *error, size_t error_len) {
	struct MemoryStruct call_result;
	char cp_api_method_url[1000];
	struct curl_httppost *post = NULL;

	call_result.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    assert(call_result.memory);
	call_result.size = 0;    /* no data at this point */ 

	curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();

	snprintf(cp_api_method_url, sizeof(cp_api_method_url), "%s%s%s%s", CP_METHOD_DATA_SERVER,
        method, CP_METHOD_DATA_ACCESS_TOKEN, access_token);
	
	/* specify URL to get authorization code */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, cp_api_method_url);

	/* Set the form info to POST (CoursePeer requires POST)*/
	curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	 
	/* we pass our 'code' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&call_result);
	 
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	 
	/* get it! */ 
	curl_easy_perform(curl_handle);
	 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);

	snprintf(result, result_len, call_result.memory);

	return 0;
}
