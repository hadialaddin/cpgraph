<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');

/*
|--------------------------------------------------------------------------
| CoursePeer Knowledge Graph Library Constants
|--------------------------------------------------------------------------
|
| Change the following constants to whatever suits your application.
|
*/
define('APP_KEY','#########'); 				// Replace ######### with your CoursePeer Application Key provided to you by CoursePeer's Support team.
define('APP_SECRET','#########'); 			// Replace ######### with your CoursePeer Application Secret provided to you by CoursePeer's Support team.
define('SITE_HOST','#########'); 			/* Replace ######### with your own site host (i.e. domain name OR IP Address of the hosting server).
										   	   Note that you must NOT include any protocol prefix (i.e. use 'www.domain.com', but NOT 'http://www.domain.com')
											 */
define('CP_APP_PATH','#########'); 			/* Replace ######### with the full path (if any), ALWAYS starting with a leading forward slash '/', where
										   	   the unarchived files and folders reside on your server. Eg. If files are hosted at:
											   'http://www.yourdomain.com/demoapp/another_folder/another', then the '#########' becomes:
											   '/demoapp/another_folder/another'. If files are hosted at 'root' (files reside on the 'www' domain folder),
											   then leave this constant empty (i.e. the '#########' becomes '').
											 */
define('CP_INTERNAL_APP_PATH','#########'); /* Replace ######### with the full path (if any), ALWAYS starting with a leading forward slash '/', where
										   	   the Internal Application (iFrame) page resides. Eg. If that page is hosted at:
											   'http://www.yourdomain.com/demoapp/another_folder/another/cpinternalapplication.php', then the '#########' becomes:
											   '/demoapp/another_folder/another/cpinternalapplication.php'. If files are hosted at 'root' (files reside on the 'www' 
											   domain folder), then leave this constant empty (i.e. the '#########' becomes '').
											 */
define('DB_HOST','#########'); 				// Replace ######### with your database server host or IP address (usually '127.0.0.1').
define('DB_USERNAME','#########'); 			// Replace ######### with your database's username.
define('DB_PASSWORD','#########'); 			// Replace ######### with your database's password.
define('DB_NAME','#########'); 				// Replace ######### with your database's name.



###########################################################################
############### NO NEED TO CHANGE ANYTHING BEYOND THIS LINE ###############
###########################################################################


/*
|--------------------------------------------------------------------------
| File and Directory Modes
|--------------------------------------------------------------------------
|
| These prefs are used when checking and setting modes when working
| with the file system.  The defaults are fine on servers with proper
| security, but you may wish (or even need) to change the values in
| certain environments (Apache running a separate process for each
| user, PHP under CGI with Apache suEXEC, etc.).  Octal values should
| always be used to set the mode correctly.
|
*/
define('FILE_READ_MODE', 0644);
define('FILE_WRITE_MODE', 0666);
define('DIR_READ_MODE', 0755);
define('DIR_WRITE_MODE', 0777);

/*
|--------------------------------------------------------------------------
| File Stream Modes
|--------------------------------------------------------------------------
|
| These modes are used when working with fopen()/popen()
|
*/

define('FOPEN_READ',							'rb');
define('FOPEN_READ_WRITE',						'r+b');
define('FOPEN_WRITE_CREATE_DESTRUCTIVE',		'wb'); // truncates existing file data, use with care
define('FOPEN_READ_WRITE_CREATE_DESTRUCTIVE',	'w+b'); // truncates existing file data, use with care
define('FOPEN_WRITE_CREATE',					'ab');
define('FOPEN_READ_WRITE_CREATE',				'a+b');
define('FOPEN_WRITE_CREATE_STRICT',				'xb');
define('FOPEN_READ_WRITE_CREATE_STRICT',		'x+b');


/* End of file constants.php */
/* Location: ./application/config/constants.php */