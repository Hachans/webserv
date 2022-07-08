#include "server.hpp"

Server::Server() : _port(PORT), _remove_poll(false), _err(false){
	// int ret = port_launch();
	// if(ret == 1)
	// {
	// 	displayAvailableServer();
	// 	run_serv();
	// }
}

Server::~Server(){
	_port_numbers.clear();
	_server_list.clear();
	_clients.clear();
}

Server::Server(int port) : _port(port), _remove_poll(false), _err(false){
	_port_numbers.push_back(port);
	int ret = port_launch();
	if(ret == 1)
	{
		displayAvailableServer();
		run_serv();
	}
}

void	Server::displayAvailableServer()
{
	std::cout << "\nAvailable servers:" << std::endl << std::endl;
	for (std::list<Server>::iterator it = _server_list.begin(); it != _server_list.end(); it++)
	{
		std::cout << "server =" << it->get_server_fd() << "= port =" << it->getPort() << "="  << std::endl;
	}
	std::cout << "\n" << std::endl;
}

void	Server::setup_err(int err, const char *msg){
	if(err < 0)
		close(_serv_fd), throw(msg);
}

int	Server::port_launch(){
	std::vector<int>& ports = get_port_numbers();
	for(_nfds = 0; _nfds < ports.size(); _nfds++){
		Server serv;
		if(ports.size() > _nfds)
			serv.set_port(ports[_nfds]);
		serv.setup_serv();
		if(serv.check_error() == true)
			return -1;
		_poll_fds[_nfds].fd = serv.get_server_fd();
		_poll_fds[_nfds].events = POLLIN;
		_server_list.push_back(serv);
	}
	return 1;
}

void	Server::setup_serv(){
	int ret;
	int opt = 1;
	try{
		_err_string = "200";
		_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
		setup_err(_serv_fd, "error creating socket");
		ret = setsockopt(_serv_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
		setup_err(ret, "error set socket");
		ret = fcntl(_serv_fd, F_SETFL, O_NONBLOCK);
		setup_err(ret, "error fcntl");
		_address.sin_family = AF_INET;// IPv4 protocol
		memset(&_address.sin_zero, 0, sizeof(_address.sin_zero));
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		ret = bind(_serv_fd, (struct sockaddr *)&_address, sizeof(_address));// Forcefully attaching socket to the port
		setup_err(ret, "error binding");
		_listen_fd = listen(_serv_fd, SIZE_POLLFD);
		setup_err(_listen_fd, "Error function  the listening");
		_http_table = http_table();
		_mime_types = initialize_mime_types();
	}
	catch(const char *msg){
		std::cout << msg << std::endl;
		_err = true;
	}
}

void	Server::run_serv(){
	int ret;
	try{
		while(true){
			ret = poll(_poll_fds, _nfds, -1);
			if(ret < 0)
				throw("error poll()");
			if(ret == 0)
				throw("poll() time out");
			for(size_t i = 0; i < _nfds ; i++){
				if(_poll_fds[i].revents == 0)
					continue;
				handle_event(i);
			}
			squeeze_poll();
		}
	}
	catch (const char *msg){
		std::cout << msg << std::endl;
	}
}

void	Server::handle_event(size_t ind){
	size_t old_size;
	for(std::list<Server>::iterator it = _server_list.begin(); it != _server_list.end(); it++){
		Server &curr_serv = *it;
		std::vector<int> &vect_client = curr_serv.get_clients();
		old_size = vect_client.size();
		if(_poll_fds[ind].fd == curr_serv.get_server_fd()){
			curr_serv.accept_connections();
			addToPollFds(vect_client, old_size);
		}
		else if((std::find(vect_client.begin(), vect_client.end(), _poll_fds[ind].fd)) != vect_client.end()){
			_end_connection = curr_serv.handle_existing_connection(&_poll_fds[ind]);
			if(_end_connection)
				_remove_poll = true;
		}
	}
}

void Server::accept_connections(){
	int new_sock = 0;
	int addrlen = sizeof(_address);
	while(new_sock != -1){
		new_sock = accept(_serv_fd, (sockaddr*)&_address, (socklen_t*)&addrlen);
		if(new_sock < 0){
			if(errno != EWOULDBLOCK)
				setup_err(new_sock, "error accept()");
			new_sock = -1;
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
		parse_first_line(std::string(_buffer));
		parse_header(_buffer);
		if (_http_request["Type"] == "POST")
			process_post_request();
		else
			create_get_response();
		poll->events = POLLOUT;
		
	}
	if(_end_connection){
		close(poll->fd);
		squeeze_client_vect(poll->fd);
		poll->fd = -1;
		_remove_client = true;
	}
	return _end_connection;
}

int	Server::send_response(struct pollfd *poll){
	static bool status = true;
	static std::string resp = _response["Header"] + _response["Date"] + _response["Server"] + _response["Content-Type"] + _response["Content-Length"] + _response["Connection"] + "\r\n" + _response["Body"];
	if(status == false)
		resp = _response["Header"] + _response["Date"] + _response["Server"] + _response["Content-Type"] + _response["Content-Length"] + _response["Connection"] + "\r\n" + _response["Body"];
	int rsize = resp.length();
	std::cout << "RESPONSE\n\n" << resp << "\n\n";
	int ret = send(poll->fd, resp.c_str(), (BUFFER_SIZE < rsize ? BUFFER_SIZE : rsize), 0);
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
	std::cout << "\n" << "===============   "  << ret << " BYTES  RECEIVED   ===============\n";
	std::cout << _buffer;
	std::cout << "======================================================\n" << std::endl;

	return ret;

}

void Server::create_get_response()
{
	std::fstream file(_http_request["Path"]);
	std::stringstream ss;
	std::stringstream ss2;

	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";
	if (!file && _err_string == "200")
		_err_string = "404";
	if (_err_string != "200")
	{
		_response["Header"] = _http_request["Version"];
		_response["Header"] += " ";
		_response["Header"] += _err_string;
		_response["Header"] += " ";
		_response["Header"] += _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = generate_html(_err_string);
		_file_size = _response["Body"].length();
		_response["Content-Length"] = "Content-Length: ";
		ss2 << _response["Body"].length();
		_response["Content-Length"] += ss2.str();
		_response["Content-Length"] += "\r\n";
		_response["Content-Type"] = "Content-Type: ";
		_response["Content-Type"] += _mime_types[".html"];
	}
	else
	{
		ss << file.rdbuf();
		_response["Header"] = _http_request["Version"];
		_response["Header"] += " 200 ";
		_response["Header"] += _http_table["200"];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = ss.str();
		_response["Content-Length"] = "Content-Length: ";
		_file_size = _response["Body"].length();
		ss2 << _response["Body"].length();
		_response["Content-Length"] += ss2.str();
		_response["Content-Length"] += "\r\n";
		_response["Content-Type"] = "Content-Type: ";
		_response["Content-Type"] += _mime_types[_http_request["Content-Type"]];
	}
	_response["Connection"] = "Connection: closed\r\n";
	_err_string = "200";
}

void	Server::process_post_request()
{
	int pos = _http_request["Content-Disposition"].find("filename=") + 10;
	std::string file_name = _http_request["Content-Disposition"].substr(pos);
	file_name = file_name.substr(0, file_name.length() - 2);

	std::ifstream file(file_name);
	if (file)
	{
		_err_string = "422";
		file.close();
		return ;
	}
	std::ofstream ofs("test_files/" + file_name);
	ofs << _http_request["Body"];

	std::stringstream len;
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";
	_response["Header"] = _http_request["Version"];
	_response["Header"] += " 200 ";
	_response["Header"] += _http_table["200"];
	_response["Server"] = "Server: Webserv\r\n";
	_response["Body"] = post_page();
	len << _response["Body"].length();
	_response["Content-Length"] += len.str();
	_response["Content-Length"] += "\r\n";
	_response["Content-Type"] = "Content-Type: ";
	_response["Content-Type"] += _mime_types[".html"];
	_response["Connection"] = "Connection: closed\r\n";
	_err_string = "200";
}
