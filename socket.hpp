#ifndef socket_hpp
#define socket_hpp

#include "server.hpp"
#include "defines.hpp"

class Socket{
	
	std::vector<conf_data*>		*_conf_vector;
	struct pollfd				_poll_fds[SIZE_POLLFD];
	size_t						_nfds;
	std::list<Server>			_server_list;
	bool						_remove_poll;
	bool						_end_connection;
	std::vector<int>			_port_numbers;
	
	public:

		Socket(std::vector<conf_data*> *vect);
		~Socket();
		void				run_serv();
		void				handle_event(size_t &ind);
		int					port_launch();
		void				displayAvailableServer();
		void				squeeze_poll();
		void				addToPollFds(std::vector<int>& vect_client, size_t old_size);
		std::vector<int>&	get_port_numbers(){ return _port_numbers; }
};


#endif
