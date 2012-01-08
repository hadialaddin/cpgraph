/***************************************************************************
 * Copyright (C) 2011, Hadi Aladdin, <support@coursepeer.com>
 * Downloaded from: http://www.github.com/hadialaddin/cpgraph
 *
 * Sample application that uses CoursePeer's Knowledge Graph API library.
 ***************************************************************************/ 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpgraph.h"

int main() {
	char returned_access_token[200];
	int access_token_result;
	
	/* We will use these variables to prompt user for their
	 * CoursePeer registered Email and Password for login.
	 */
	char email[200];
	char password[200];

	// Error variable for server requests
	char error[500];

	/* Will use this variable to store the result of the
	 * CoursePeer API method call.
	 */
	char* cp_method_result = malloc(1000000);
    assert(cp_method_result);

	// Prompt user to input CoursePeer Account credentials
	printf("Login to your CoursePeer account\n");
	printf("Email: ");
    fgets(email, sizeof(email), stdin);
    if (email[strlen(email) - 1] == '\n')
      email[strlen(email) - 1] = 0;

	printf("Password: ");
    fgets(password, sizeof(password), stdin);
    if (password[strlen(password) - 1] == '\n')
      password[strlen(password) - 1] = 0;

	// Aquire the access_token from the CoursePeer server
	access_token_result = request_access_token(returned_access_token, email, password, error);

	// Check if the returned value is 0 (failed)
	if(access_token_result == 1) {
		fprintf(stderr, "%s\n", error);
        goto cleanup;
    }

    /* returned_access_token contains the aquired access token for this session
    * Do whatever you want with it. I.e. send requests to the CoursePeer server
    * to implement methods. See sample below.
    */

    // Print the access_token
    printf("\nThe aquired access_token for this session: %s\n", returned_access_token);

    /* NOTE: you can store the access_token in a text file for future retrieval (history)
    * so that you don't ask the user to login everytime he/she opens your application.
    * However, remember that the access_token expires in a specific period of time,
    * by which you need to aquire a new access_token.
    */

    // ############################################################################### //

    // User Profile
    printf("\n#########################\nUser Profile:\n");
    cp_api_method(cp_method_result, "/users/me/profile", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';

    // User Courses
    printf("\n#########################\nUser Courses:\n");
    cp_api_method(cp_method_result, "/users/me/courses", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';

    // User Questions
    printf("\n#########################\nUser Questions:\n");
    cp_api_method(cp_method_result, "/users/me/questions", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';

    // User Bookmarks
    printf("\n#########################\nUser Bookmarks:\n");
    cp_api_method(cp_method_result, "/users/me/bookmarks", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';

    // User Tips
    printf("\n#########################\nUser Tips:\n");
    cp_api_method(cp_method_result, "/users/me/tips", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';
    
    // User Life Activities/Places
    printf("\n#########################\nUser Life Activities/Places:\n");
    cp_api_method(cp_method_result, "/users/me/life", returned_access_token, error);
    printf("\n%s\n\n", cp_method_result);
    /* Clear result variable to use in next API call */
    *cp_method_result = '\0';

cleanup:
	// Keep it clean!
	free(cp_method_result);

	return 0;
}
