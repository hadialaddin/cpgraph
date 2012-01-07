/***************************************************************************
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 * Modified by Hadi Aladdin (support@coursepeer.com)
 * Downloaded from: http://www.github.com/hadialaddin/cpgraph
 *
 * Implementation for the C library for CoursePeer's Knowledge Graph API
 * using libcurl
 ***************************************************************************/ 

// YOU MUST MODIFY THESE DEFINITIONS TO SUIT YOUR APPLICATION
#define CP_APP_KEY "PUT YOUR APP_KEY HERE"
#define CP_APP_SECRET "PUT YOUR APP_SECRET HERE"

// DO NOT MODIFY ANYTHING BELOW THIS LINE
#define CP_APP_REDIRECT_URI "libcurl"

#define CP_AUTHORIZE_SERVER "http://graph.coursepeer.com/index.php/oauth/"
#define CP_ACCESS_TOKEN_GRANT_SERVER "http://graph.coursepeer.com/index.php/oauth/access_token/"
#define CP_METHOD_DATA_SERVER "http://graph.coursepeer.com/index.php/data"

#define CP_REQUEST_CODE_URI_STARTER "?response_type=code&scope=basic"
#define CP_REQUEST_CODE_URI_CLIENTID "&client_id="
#define CP_REQUEST_CODE_URI_REDIRECT_URI "&redirect_uri="
#define CP_REQUEST_CODE_URI_EMAIL "&email="
#define CP_REQUEST_CODE_URI_PASSWORD "&password="
#define CP_REQUEST_CODE_URI_DESKTOP "&desktop=1"

#define CP_ACCESS_TOKEN_URI_STARTER "?grant_type=authorization_code"
#define CP_ACCESS_TOKEN_URI_CLIENTID "&client_id="
#define CP_ACCESS_TOKEN_URI_CLIENTSECRET "&client_secret="
#define CP_ACCESS_TOKEN_URI_REDIRECT_URI "&redirect_uri="
#define CP_ACCESS_TOKEN_URI_CODE "&code="
#define CP_ACCESS_TOKEN_URI_DESKTOP "&desktop=1"

#define CP_METHOD_DATA_ACCESS_TOKEN "?access_token="

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
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	 
	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
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
 * Returns 1 if failed, and 0 if success.
 */
int request_access_token(char * returned_access_token, char * email, char * password, char * error)
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
	json_t *access_token_value;
	 
	code.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	code.size = 0;    /* no data at this point */ 

	access_token_response.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	access_token_response.size = 0;    /* no data at this point */ 
	 
	curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();

	sprintf(cp_code_url,"%s%s%s%s%s%s%s%s%s%s%s%s",CP_AUTHORIZE_SERVER,CP_REQUEST_CODE_URI_STARTER,CP_REQUEST_CODE_URI_CLIENTID,CP_APP_KEY,CP_REQUEST_CODE_URI_REDIRECT_URI,CP_APP_REDIRECT_URI,CP_REQUEST_CODE_URI_EMAIL,email,CP_REQUEST_CODE_URI_PASSWORD,password,CP_REQUEST_CODE_URI_DESKTOP,"\n");
	
/* specify URL to get authorization code */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, cp_code_url);
	 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	 
	/* we pass our 'code' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&code);
	 
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	 
	/* get it! */ 
	curl_easy_perform(curl_handle);
	 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);

	if(strcmp("authentication_failed",code.memory)==0){
		// Authentication Failed
		strcpy(error, "Authentication Failure. The provided Email and Password are incorrect.");
		return 1; // Failed to authenticate user credentials
	}
	// Now, our code.memory points to a memory block that is code.size
	// bytes big and contains the CoursePeer Code. Next, is to exchange
	// this code for an access_token.

	sprintf(access_token_granting_url,"%s%s%s%s%s%s%s%s%s%s%s",CP_ACCESS_TOKEN_GRANT_SERVER,CP_ACCESS_TOKEN_URI_STARTER,CP_ACCESS_TOKEN_URI_CLIENTID,CP_APP_KEY,CP_ACCESS_TOKEN_URI_CLIENTSECRET,CP_APP_SECRET,CP_ACCESS_TOKEN_URI_REDIRECT_URI,CP_APP_REDIRECT_URI,CP_ACCESS_TOKEN_URI_CODE,code.memory,CP_ACCESS_TOKEN_URI_DESKTOP);

	 curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();
	 
	/* specify URL to get */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, access_token_granting_url);
	 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	 
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
		strcpy(error, "Something went wrong with the authorization server. Please try again later.");
		return 1;
	}

	access_token = json_object_get(access_token_json, "access_token");

	// Keep memory clean before we return
	if(code.memory)
		free(code.memory);

	if(access_token_response.memory)
		free(access_token_response.memory);
 
	/* we're done with libcurl, so clean it up */ 
	curl_global_cleanup();

	// Make sure we have the access token and return, otherwise
	// return error code
	if (access_token && json_is_string(access_token)) {
		// Pointer is value until we do the next name refresh
		strcpy(returned_access_token, json_string_value(access_token));
		return 0;
	}
	else{
		// Something went wrong, return error code
		strcpy(error, "Something went wrong. Try again later.");
		return 1;
	}

}

int cp_api_method(char * result, char * method, char * access_token, char * error){

	struct MemoryStruct call_result;
	char cp_api_method_url[1000];
	struct curl_httppost *post=NULL;

	call_result.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	call_result.size = 0;    /* no data at this point */ 

	curl_global_init(CURL_GLOBAL_ALL);
	 
	/* init the curl session */ 
	curl_handle = curl_easy_init();

	sprintf(cp_api_method_url,"%s%s%s%s",CP_METHOD_DATA_SERVER,method,CP_METHOD_DATA_ACCESS_TOKEN,access_token);
	
	/* specify URL to get authorization code */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, cp_api_method_url);

	/* Set the form info to POST (CoursePeer requires POST)*/
	curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
 
	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	 
	/* we pass our 'code' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&call_result);
	 
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	 
	/* get it! */ 
	curl_easy_perform(curl_handle);
	 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);

	strcpy(result,call_result.memory);

	return 0;
}