<?php
/**
 * OAuth 2.0 client for use with the included auth server
 *
 * @author              Alex Bilbie | www.alexbilbie.com | alex@alexbilbie.com
 * @copyright   		Copyright (c) 2011, Alex Bilbie.
 * @license             http://www.opensource.org/licenses/mit-license.php
 * @link                https://github.com/alexbilbie/CodeIgniter-OAuth-2.0-Server
 * @version             Version 0.2
 * @modified by			Hadi Aladdin (support@coursepeer.com)
 */
 
/*
	Copyright (c) 2011 Alex Bilbie | alex@alexbilbie.com
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

class Signin extends CI_Controller {
	
	function __construct()
	{
		parent::__construct();
	}
		
	function index()
	{
		$this->load->library('oauth_client');
		$this->oauth_client->sign_in();
	}
	
	function signout()
	{

		session_start();
		unset($_SESSION['cp_access_token']);
		$this->oauth_client->sign_out();
	}
	
	function redirect()
	{
		$this->load->library('oauth_client');
		
		// If there was a problem with the auth server or the user declined your application there will be an error
		if ($this->input->get('error'))
		{
			if($this->input->get('internalapp')=='1')
				header('Location: '.(CP_INTERNAL_APP_PATH==''?'/':CP_INTERNAL_APP_PATH).'?error='.$this->input->get('error'));
			else
				header('Location: '.(CP_APP_PATH==''?'/':CP_APP_PATH).'?error='.$this->input->get('error'));
			
		}
						
		elseif ($this->input->get('code'))
		{
			$code = trim($this->input->get('code'));
			$state = trim($this->input->get('state'));
			
			// Convert the states back to an array and validate the CSRF token
			if ($state !== "")
			{
				$states = explode('&', urldecode($state));
				$state_params = array();
				
				foreach($states as $v)
				{
					$s = explode('=', $v);
					$state_params[$s[0]] = $s[1];
				}
				
				// Validate the CSRF token
				if (isset($state_params['oauth_csrf']))
				{
					if ($state_params['oauth_csrf'] !== $this->session->userdata('oauth_csrf'))
					{
						show_error('The state does not match. You may be a victim of CSRF.');
					}
				}
			}
			
			$access_token = $this->oauth_client->get_access_token($code);
			
			if ($access_token)
			{
				// Check to see a user already exists in your app's database based by searching for the access token
				
				// Get the Profile data from CoursePeer Knowledge Graph API
				
				// Initialize the REST call (don't change)
				$this->rest->initialize(array('server' => config_item('base_url')));
				// Execute your CoursePeer Graph API Method Call after 'index.php/data' segment - Result returned as a JSON associative array
				$cp_profile = $this->rest->post('index.php/data/users/me/profile?access_token='.$access_token,array());
				
				// Check against your own database of the application whether the user is new or not
				$query = $this->db->query("SELECT userid FROM cpapp_users WHERE cpuserid=".$cp_profile['user_id'], FALSE);
				$user_exists = $query->num_rows();
				if ($user_exists>0)
				{
					// The user exists already, update access_token
					$this->db->query("UPDATE cpapp_users SET ".($this->input->get('internalapp')==1?"cp_internal_access_token":"cp_access_token")."='".$access_token."' WHERE cpuserid=".$cp_profile['user_id'], FALSE);
					// Set up their sessions and redirect them to the app
					session_start();
					if($this->input->get('internalapp')=='1'){
						$_SESSION['cp_internal_access_token']=$access_token;
						$_SESSION['cp_internal_userid']=$cp_profile['user_id'];
						$_SESSION['cp_internal_name']=$cp_profile['name'];
						$_SESSION['cp_internal_usertype']=$cp_profile['user_type'];
						$_SESSION['cp_internal_profile_photo_url']=$cp_profile['profile_photo_url'];
						$_SESSION['cp_internal_homecountry']=$cp_profile['home_country'];
						$_SESSION['cp_internal_school']=$cp_profile['school'];
						$_SESSION['cp_internal_major']=$cp_profile['major'];
						foreach ($query->result() as $user){
							$_SESSION['internal_userid']=$user->userid;
						}
					}
					else{
						$_SESSION['cp_access_token']=$access_token;
						$_SESSION['cp_userid']=$cp_profile['user_id'];
						$_SESSION['cp_name']=$cp_profile['name'];
						$_SESSION['cp_usertype']=$cp_profile['user_type'];
						$_SESSION['cp_profile_photo_url']=$cp_profile['profile_photo_url'];
						$_SESSION['cp_homecountry']=$cp_profile['home_country'];
						$_SESSION['cp_school']=$cp_profile['school'];
						$_SESSION['cp_major']=$cp_profile['major'];
						foreach ($query->result() as $user){
							$_SESSION['userid']=$user->userid;
						}

					}

					if($this->input->get('internalapp')=='1')
						header('Location: '.(CP_INTERNAL_APP_PATH==''?'/':CP_INTERNAL_APP_PATH));
					else
						header('Location: '.(CP_APP_PATH==''?'/':CP_APP_PATH));
				}
				
				else
				{
					// Insert the user's details into your app's database (most importantly, the access token)
					$this->db->query("INSERT INTO cpapp_users (cpuserid,".($this->input->get('internalapp')==1?"cp_internal_access_token":"cp_access_token").",name,usertype) VALUES (".$cp_profile['user_id'].",'".$access_token."','".$cp_profile['name']."',".($cp_profile['user_type']=="Student"?1:2).")", FALSE);
					// Set up their sessions and redirect them to the app
					session_start();
					if($this->input->get('internalapp')=='1'){
						$_SESSION['cp_internal_access_token']=$access_token;
						$_SESSION['cp_internal_userid']=$cp_profile['user_id'];
						$_SESSION['cp_internal_name']=$cp_profile['name'];
						$_SESSION['cp_internal_usertype']=$cp_profile['user_type'];
						$_SESSION['cp_internal_profile_photo_url']=$cp_profile['profile_photo_url'];
						$_SESSION['cp_internal_homecountry']=$cp_profile['home_country'];
						$_SESSION['cp_internal_school']=$cp_profile['school'];
						$_SESSION['cp_internal_major']=$cp_profile['major'];
						$_SESSION['internal_userid']=$this->db->insert_id();
					}
					else{
						$_SESSION['cp_access_token']=$access_token;
						$_SESSION['cp_userid']=$cp_profile['user_id'];
						$_SESSION['cp_name']=$cp_profile['name'];
						$_SESSION['cp_usertype']=$cp_profile['user_type'];
						$_SESSION['cp_profile_photo_url']=$cp_profile['profile_photo_url'];
						$_SESSION['cp_homecountry']=$cp_profile['home_country'];
						$_SESSION['cp_school']=$cp_profile['school'];
						$_SESSION['cp_major']=$cp_profile['major'];
						$_SESSION['internal_userid']=$this->db->insert_id();
					}
					
					if($this->input->get('internalapp')=='1')
						header('Location: '.(CP_INTERNAL_APP_PATH==''?'/':CP_INTERNAL_APP_PATH));
					else
						header('Location: '.(CP_APP_PATH==''?'/':CP_APP_PATH));
					
				}
			}
			
			else
			{
				show_error($this->oauth_client->error, 500);
			}
		
		}
		
		else
		{
			// No authorise code or error code redirect them for trying to be a fool
			$this->load->helper('url');
			redirect(site_url() . '/oauth/sign_in');
		}
	}

} // EOF
