#include "server.hpp"

Server::Server() : _port(PORT), _remove_poll(false), _err(false){

}

Server::~Server(){

}

Server::Server(int port) : _port(port), _remove_poll(false), _err(false){

}

void	Server::setup_err(int err, const char *msg){
	if(err < 0)
		close(_serv_fd), throw(msg);
}

void	Server::setup_serv(){
	int ret;
	int opt = 1;
	_nfds = 0;
	try{
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
	}
	catch(const char *msg){
		std::cout << msg << std::endl;
		_err = true;
	}
	_http_table = http_table();
	_mime_types = initialize_mime_types();
	_poll_fds[_nfds].fd = get_server_fd();
	_poll_fds[_nfds].events = POLLIN;
	_nfds++;
	this->run_serv();
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

void	Server::squeeze_poll()
{
	if (_remove_poll){
		_remove_poll = false;
		for (size_t i = 0; i < _nfds; i++){
			if (_poll_fds[i].fd == -1){
				for(size_t j = i; j < _nfds - 1; j++){
					_poll_fds[j].fd = _poll_fds[j+1].fd;
				}
				i--;
				_nfds--;
			}
		}
	}
}

void	Server::handle_event(size_t ind){
	size_t old_size;
	std::vector<int> &vect_client = this->get_clients();
	old_size = vect_client.size();
	if(_poll_fds[ind].fd == this->get_server_fd()){
		this->accept_connections();
		addToPollFds(vect_client, old_size);
	}
	else if((std::find(vect_client.begin(), vect_client.end(), _poll_fds[ind].fd)) != vect_client.end()){
		_end_connection = this->handle_existing_connection(&_poll_fds[ind]);
		if(_end_connection)
			_remove_poll = true;
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

void Server::addToPollFds(std::vector<int>& vect_client, size_t old_size){
	for(size_t i = old_size; i < vect_client.size(); i++){
		_poll_fds[_nfds].fd = vect_client[i];
		_poll_fds[_nfds].events = POLLIN;
		_poll_fds[_nfds].revents = 0;
		_nfds++;
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
		
		create_get_response();
		check_values();
		
	}
	if(_end_connection){
		close(poll->fd);
		// squeeze_client_vect(poll->fd);
		poll->fd = -1;
		_remove_client = true;
	}
	return _end_connection;
}

int	Server::send_response(struct pollfd *poll){
	std::string page = basic_page();
	std::string resp = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: ";
	resp += page.length();
	resp += "\r\n\r\n";
	resp += page;
	resp += "\r\n";

	int ret = send(poll->fd, resp.c_str(), resp.length(), 0);
	if(ret < 0)
		return ret;
	std::cout << "successfully sent " << ret << " bytes\n";
	poll->events = POLLIN;
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

	poll->events = POLLOUT;
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
	if (!file)
	{
		_response["Header"] = _http_request["Version"];
		_response["Header"] += " 404 ";
		_response["Header"] += _http_table["404"];
		_response["Server"] += "Server: Webserv\r\n";
		_response["Body"] = generate_html("404");
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
		_response["Server"] += "Server: Webserv\r\n";
		_response["Body"] = ss.str();
		_response["Content-Length"] = "Content-Length: ";
		ss2 << _response["Body"].length();
		_response["Content-Length"] += ss2.str();
		_response["Content-Length"] += "\r\n";
		_response["Content-Type"] = "Content-Type: ";
		_response["Content-Type"] += _mime_types[_http_request["Content-Type"]];
	}
}
