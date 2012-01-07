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

#include <stdio.h>
#include <stdlib.h> 
#include <stdarg.h>
 
/* Function used as a callback for the cURL;
 * gets executed when it returns back from the call.
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

/* 
 * Places the access_token value in the
 * passed 'return_access_token' char array.
 * Returns 1 if failed, and 0 if success.
 */
int request_access_token(char * returned_access_token, char * email, char * password, char * error);

/* 
 * Executes the CoursePeer API Call for 
 * the passed 'method' using 'access_token'.
 * The result is stored in 'result'.
 * Returns 1 if failed, and 0 if success.
 */
int cp_api_method(char * result, char * method, char * access_token, char * error);

#endif