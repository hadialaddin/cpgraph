<?php defined('BASEPATH') OR exit('No direct script access allowed');
/**
 * CodeIgniter REST Class
 *
 * Mske REST requests to RESTful services with simple syntax.
 *
 * @package        	CodeIgniter
 * @subpackage    	Libraries
 * @category    	Libraries
 * @author        	Philip Sturgeon
 * @created			04/06/2009
 * @license         http://philsturgeon.co.uk/code/dbad-license
 * @link			http://getsparks.org/packages/restclient/show
 */

class REST
{
    protected $_ci;

	protected $rest_server;
	protected $mime_type;
	
	protected $http_auth = null;
	protected $http_user = null;
	protected $http_pass = null;

    protected $response_string;

    function __construct($config = array())
    {
        $this->_ci =& get_instance();
        log_message('debug', 'REST Class Initialized');

		$this->_ci->load->library('curl');

		// If a URL was passed to the library
		empty($config) OR $this->initialize($config);
    }

	function __destruct()
	{
		$this->_ci->curl->set_defaults();
	}

    public function initialize($config)
    {
		$this->rest_server = @$config['server'];

		if (substr($this->rest_server, -1, 1) != '/')
		{
			$this->rest_server .= '/';
		}

		isset($config['http_auth']) && $this->http_auth = $config['http_auth'];
		isset($config['http_user']) && $this->http_user = $config['http_user'];
		isset($config['http_pass']) && $this->http_pass = $config['http_pass'];
    }


    public function get($uri, $params = array())
    {
        if ($params)
        {
        	$uri .= '?'.(is_array($params) ? http_build_query($params) : $params);
        }

    	return $this->_call('get', $uri, NULL);
    }


    public function post($uri, $params = array())
    {
        return $this->_call('post', $uri, $params);
    }


    public function put($uri, $params = array())
    {
        return $this->_call('put', $uri, $params);
    }


    public function delete($uri, $params = array())
    {
        return $this->_call('delete', $uri, $params);
    }

    public function api_key($key, $name = 'X-API-KEY')
	{
		$this->_ci->curl->http_header($name, $key);
	}

    public function language($lang)
	{
		if (is_array($lang))
		{
			$lang = implode(', ', $lang);
		}

		$this->_ci->curl->http_header('Accept-Language', $lang);
	}

    protected function _call($method, $uri, $params = array())
    {

		$this->_set_headers();

        // Initialize cURL session
        $this->_ci->curl->create($this->rest_server.$uri);

        // If authentication is enabled use it
        if ($this->http_auth != '' && $this->http_user != '')
        {
        	$this->_ci->curl->http_login($this->http_user, $this->http_pass, $this->http_auth);
        }

        // We still want the response even if there is an error code over 400
        $this->_ci->curl->option('failonerror', FALSE);

        // Call the correct method with parameters
        $this->_ci->curl->{$method}($params);

        // Execute and return the response from the REST server
        $response = $this->_ci->curl->execute();

        // Format and return
        return $this->_json($response);
    }


	// Return HTTP status code
	public function status()
	{
		return $this->info('http_code');
	}

	// Set custom options
	public function option($code, $value)
	{
		$this->_ci->curl->option($code, $value);
	}

	protected function _set_headers()
	{
		$this->_ci->curl->http_header('Accept: '.$this->mime_type);
	}


    // Encode as JSON
    protected function _json($string)
    {
    	return json_decode(trim($string),true);
    }
}

/* End of file REST.php */
/* Location: ./application/libraries/REST.php */