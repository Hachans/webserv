#include "server.hpp"

Server::Server(conf_data *data) : _port(PORT), _finished(false), _err(false) , _data(data), _dir(false){
	_data_vec.push_back(data);
	size_t j;
	std::string str = _data->CGI_extensions;
	while (_data->CGI_extensions != "")
	{
		j = str.find(" ");
		_cgi_types.push_back(str.substr(0, j));
		str.erase(0, j+1);
		if (j == std::string::npos)
			break;
	}
}

Server::~Server(){
}


void	Server::setup_err(int err, const char *msg){
	if(err < 0)
		close(_serv_fd), throw(msg);
}

void	Server::setup_serv(){
	int ret;
	int opt = 1;
	try{
		_err_string = "200";
		_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
		setup_err(_serv_fd, "error socket()");
		ret = setsockopt(_serv_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
		setup_err(ret, "error setsockopt()");
		ret = fcntl(_serv_fd, F_SETFL, O_NONBLOCK);
		setup_err(ret, "error fcntl()");
		_address.sin_family = AF_INET;
		memset(&_address.sin_zero, 0, sizeof(_address.sin_zero));
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		if(_data->s_host() != "localhost")
			_address.sin_addr.s_addr = inet_addr(_data->s_host().c_str());
		ret = bind(_serv_fd, (struct sockaddr *)&_address, sizeof(_address));
		// setup_err(ret, "error bind()");
		if (ret == -1)
		{
			// perror("bind");
			close(_serv_fd), throw("bind error");
		}
		_listen_fd = listen(_serv_fd, SIZE_POLLFD);
		setup_err(_listen_fd, "error listen()");
		_http_table = http_table();
		_mime_types = initialize_mime_types();
	}
	catch(const char *msg){
		std::cout << msg << std::endl;
		_err = true;
	}
}


void Server::accept_connections(){
	int new_sock = 0;
	int addrlen = sizeof(_address);
	while(new_sock != ERR){
		new_sock = accept(_serv_fd, (sockaddr*)&_address, (socklen_t*)&addrlen);
		if(new_sock < 0){
			if(errno != EWOULDBLOCK)
				setup_err(new_sock, "error accept()");
			new_sock = ERR;
		}
		else{
			std::cout << "new client:" << new_sock << std::endl;
			_clients.push_back(new_sock);
		}
		
	}
}

bool	Server::handle_existing_connection(struct pollfd *poll){
	int ret;
	_end_connection = false;

	if(poll->revents & POLLOUT){
		ret = send_response(poll);
		if(ret < 0)
			perror("send");
		else if(ret == 0){
			close(poll->fd);
			_end_connection = true;
		}
	}
	else if((ret = recieve_data(poll)) > 0){
		if(_storage_data == ""){
			std::string temp = _storage.substr(_storage.find("Host: ") + 6);
			_http_request["Host"] = temp.substr(0, temp.find("\r\n"));
		}

		// _http_request["HOST"] = "deus";
		for (std::vector<conf_data*>::const_iterator it = _data_vec.begin(); it != _data_vec.end(); ++it)
		{
			std::string names = (*it)->s_names();
			std::string n;
			while (names != "")
			{
				n = names.substr(0, names.find(' '));
				if (n == _http_request["Host"]){
					_data = *it;
					size_t j;
					_cgi_types.clear();
					std::string str = _data->CGI_extensions;
					while (_data->CGI_extensions != "")
					{
						j = str.find(" ");
						_cgi_types.push_back(str.substr(0, j));
						str.erase(0, j+1);
						if (j == std::string::npos)
							break;
					}
					break;
				}

				if (names.find(' ') != names.npos)
					names.erase(0, names.find(' ') + 1);
				else
					names.clear();
			}
		}

		std::cout << "body_size: " << _data->s_bodySize() << std::endl;
		if (_storage_data == "")
		{
			parse_first_line(std::string(_buffer));
			parse_header(_buffer);
			
		}
		process_request();
		if(_finished == true){
			poll->events = POLLOUT;
			_storage = "";
			_storage_data = "";
			_finished = false;
		}
	}
	if(_end_connection){
		close(poll->fd);
		squeeze_client_vect(poll->fd);
		poll->fd = REM;
		_remove_client = true;
	}
	_data = _data_vec[0];
	size_t j;
	_cgi_types.clear();
	std::string str = _data->CGI_extensions;
	while (_data->CGI_extensions != "")
	{
		j = str.find(" ");
		_cgi_types.push_back(str.substr(0, j));
		str.erase(0, j+1);
		if (j == std::string::npos)
			break;
	}
	return _end_connection;
}

std::map<std::string, std::string> Server::getCgiEnv(void)
{
	std::map<std::string, std::string> env;
	std::map<std::string, std::string>::iterator it;
	for (it = _http_request.begin(); it != _http_request.end(); it++)
	{
		if (it->first == "Authorization")
			env["AUTH_TYPE"] = it->second;
		else if (it->first == "Content-Length")
			env["CONTENT_LENGTH"] = it->second;
		else if (it->first == "Content-Type")
			env["CONTENT_TYPE"] = it->second;
		else if (it->first == "Path")
		{
			(it->second.find("?") != std::string::npos) ? env["QUERY_STRING"] = it->second.substr(it->second.find("?") + 1) : env["QUERY_STRING"] = "";
			env["SCRIPT_NAME"] = it->second.substr(0, it->second.find("?"));
			// if (env["SCRIPT_NAME"].rfind("/") != std::string::npos)
			// 	env["SCRIPT_NAME"] = env["SCRIPT_NAME"].substr(env["SCRIPT_NAME"].rfind("/"));
			env["PATH_INFO"] = env["SCRIPT_NAME"];
		}
		else if (it->first == "Type")
			env["REQUEST_METHOD"] = it->second;
		else if (it->first == "Accept")
			env["HTTP_ACCEPT"] = it->second;
		else if (it->first == "Host")
			env["HTTP_HOST"] = it->second;
		else if (it->first == "User-Agent")
			env["HTTP_USER_AGENT"] = it->second;
	}
	return (env);
}

void Server::process_request(){
	size_t pos;
	if(_http_request["Type"] == "GET" && (pos = _data->s_methods().find("GET") != std::string::npos))
		process_get_request();
	else if (_http_request["Type"] == "POST" && (pos = _data->s_methods().find("POST") != std::string::npos))
		process_post_request();
	else if(_http_request["Type"] == "DELETE" && (pos = _data->s_methods().find("DELETE") != std::string::npos))
		process_delete_request();
	else if(_http_request["Type"] == "HEAD" || _http_request["Type"] == "PUT" || _http_request["Type"] == "CONNECT" || _http_request["Type"] == "TRACE" || _http_request["Type"] == "PATCH" || _http_request["Type"] == "OPTIONS")
	{
		_err_string = "501";
		process_get_request();
	}
	else{
		if((pos = _data->s_methods().find(_http_request["Type"])) == std::string::npos)
			_err_string = "405";
		else
			_err_string = "400";
		process_get_request();
	}
}

int	Server::send_response(struct pollfd *poll){
	static bool status = true;
	static std::string resp = _response["Header"] + _response["Date"] + _response["Server"] + _response["Content-Type"] + _response["Content-Length"] + _response["Connection"] + "\r\n" + _response["Body"];
	if(status == false && !_is_cgi)
		resp = _response["Header"] + _response["Date"] + _response["Server"] + _response["Content-Type"] + _response["Content-Length"] + _response["Connection"] + "\r\n" + _response["Body"];
	else if (_is_cgi)
	{
		resp = _cgi_response;
		_is_cgi = false;
	}
	int rsize = resp.length();
	int ret = send(poll->fd, resp.c_str(), (BUFFER_SIZE < rsize ? BUFFER_SIZE : rsize), 0);
	_http_request["Content-Type"] = "";
	_http_request["Content-Disposition"] = "";
	if(ret < 0)
		return ret;
	std::cout << "successfully sent " << ret << " bytes remaining: " << rsize - ret << std::endl;

	resp.erase(0, ret);
	status = true;
	
	if(rsize <= 0){
		status = false;
		poll->events = POLLIN;
	}
	return ret;
}

int Server::recieve_data(struct pollfd	*poll){
	int ret = recv(poll->fd, _buffer, BUFFER_SIZE, 0);
	if(ret < 0){
		_end_connection = true;
		std::cout << "error recv()";
		return ret;
	}
	if(ret == 0){
		std::cout << "client closed connection\n";
		_end_connection = true;
		return ret;
	}
	_buffer[ret] = '\0';
	std::stringstream ss;
	for (int i = 0; i < ret; i++)
		ss << _buffer[i];
	_storage = ss.str();
	std::cout << "\n" << "===============   "  << ret << " BYTES  RECEIVED   ===============\n";
	std::cout << _buffer;
	std::cout << "======================================================\n" << std::endl;

	return ret;
}

void	Server::squeeze_client_vect(int to_find)
{
	for (std::vector<int>::iterator it = _clients.begin(); it !=  _clients.end() ; it++){
		if (*it == to_find){
			_clients.erase(it);
			return ;
		}
	}
}
