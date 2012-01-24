/***************************************************************************
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 * Modified by Hadi Aladdin (support@coursepeer.com)
 * Downloaded from: http://www.github.com/hadialaddin/cpgraph
 *
 * cpgraph.h header file for the library for CoursePeer's Knowledge Graph API
 * using libcurl
 ***************************************************************************/ 
#ifndef CPGRAPH_H
#define CPGRAPH_H
 
/* Function used as a callback for the cURL;
 * gets executed when it returns back from the call.
 */
static size_t write_memory_callback(const void *contents, size_t size, size_t nmemb, void *userp);

/* 
 * Places the access_token value in the
 * passed 'return_access_token' char array.
 * Returns 1 if failed, and 0 if success.
 */
int request_access_token(char *returned_access_token, size_t returned_access_token_len, char *email, char *password, char *error, size_t error_len);

/* 
 * Executes the CoursePeer API Call for 
 * the passed 'method' using 'access_token'.
 * The result is stored in 'result'.
 * Returns 1 if failed, and 0 if success.
 */
int cp_api_method(char *result, size_t result_len, char *method, char *access_token, char *error, size_t error_len, char * upload, char * arguments_keys[], char * arguments_values[], int arguments_count);

#endif
