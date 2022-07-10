#include "socket.hpp"

Socket::Socket(std::vector<conf_data*> *vect) : _conf_vector(vect){
	int ret = port_launch();
	if(ret == 1)
	{
		displayAvailableServer();
		run_serv();
	}
}

Socket::~Socket(){

}

int	Socket::port_launch(){
	for(_nfds = 0; _nfds < _conf_vector[0].size(); _nfds++){
		Server serv(_conf_vector[0][_nfds]);
		if(_conf_vector[0].size() > _nfds)
			serv.set_port(serv.get_data()->s_port());
		serv.setup_serv();
		if(serv.check_error() == true)
			return ERR;
		_poll_fds[_nfds].fd = serv.get_server_fd();
		_poll_fds[_nfds].events = POLLIN;
		_server_list.push_back(serv);
	}
	return 1;
}

void	Socket::run_serv(){
	int ret;
	try{
		while(true){
			ret = poll(_poll_fds, _nfds, TOUT);
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
	catch(std::exception const &e){
		std::cout << e.what() << std::endl;
	}
}


void	Socket::handle_event(size_t &ind){
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

void	Socket::displayAvailableServer()
{
	std::cout << "\nAvailable servers:" << std::endl << std::endl;
	for (std::list<Server>::iterator it = _server_list.begin(); it != _server_list.end(); it++)
		std::cout << "server =" << it->get_server_fd() << "= port =" << it->getPort() << "="  << std::endl;
	std::cout << "\n" << std::endl;
}

void	Socket::squeeze_poll()
{
	if (_remove_poll){
		_remove_poll = false;
		for (size_t i = 0; i < _nfds; i++){
			if (_poll_fds[i].fd == REM){
				for(size_t j = i; j < _nfds - 1; j++){
					_poll_fds[j].fd = _poll_fds[j+1].fd;
				}
				i--;
				_nfds--;
			}
		}
	}
}

void Socket::addToPollFds(std::vector<int>& vect_client, size_t old_size){
	for(size_t i = old_size; i < vect_client.size(); i++){
		_poll_fds[_nfds].fd = vect_client[i];
		_poll_fds[_nfds].events = POLLIN;
		_poll_fds[_nfds].revents = 0;
		_nfds++;
	}
}
