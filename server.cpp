#include "server.hpp"

Server::Server() : _port(PORT), _err(false){

}

Server::~Server(){

}

Server::Server(int port) : _port(port), _err(false){

}

void	Server::setup_err(int err, const char *msg){
	if(err < 0)
		close(_serv_fd); throw(msg);
}

void	Server::setup_serv(){
	int ret;
	int opt = 1;
	try{
		_serv_fd = socket(AF_UNSPEC, SOCK_STREAM, 0);
		setup_err(_serv_fd, "error creating socket");
		ret = setsockopt(_serv_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
		setup_err(ret, "error set socket");
		ret = fcntl(_serv_fd, F_SETFL, O_NONBLOCK);
		setup_err(ret, "error fcntl");
		_address.sin_family = AF_INET;// IPv4 protocol
		memset(&_address.sin_zero, 0, sizeof (_address.sin_zero));
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
}

void	Server::run_serv(){
	int ret;
	try{
		while(true){
			ret = poll(_poll_fds, _nfds, -1);
			setup_err(ret, "error poll");
			if(ret == 0)
				throw("poll() time out");
			for(size_t i = 0; i < _nfds ; i++){
				if(_poll_fds[i].revents == 0)
					continue;
				// handle_event(i);
			}
			// squeeze_poll();/
		}
	}
	catch (const char *msg){
		std::cout << msg << std::endl;
	}
}

// void Server::handle_event(){

// }

// void Server::squeeze_poll(){

// }
