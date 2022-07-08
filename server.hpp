#ifndef server_hpp
#define server_hpp

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For read
#include <fcntl.h> // For fcntl
#include <netdb.h> // For addrinfo
#include <arpa/inet.h> // For inet_ntop
#include <stdio.h> // For fclose()
#include <poll.h>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <sstream> // For stringstream
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <iterator>
#include <ctime>

#define PORT		8080
#define SIZE_POLLFD	30000
#define BUFFER_SIZE	8192

class Server{
	int									_serv_fd;
	int									_listen_fd;
	char								_buffer[BUFFER_SIZE + 1];
	int									_port;
	bool								_end_connection;
	bool								_remove_client;
	bool								_remove_poll;
	bool								_err;
	std::vector<int>					_clients;
	struct sockaddr_in					_address;
	struct pollfd						_poll_fds[SIZE_POLLFD];
	size_t								_nfds;
	std::map<std::string, std::string>	_http_request;
	std::map<std::string, std::string>	_response;
	std::map<std::string, std::string>	_http_table;
	std::map<std::string, std::string>	_mime_types;
	int									_file_size;
	int									_file_offset;
	std::vector<int>					_port_numbers;
	std::list<Server>					_server_list;
	std::string							_err_string;
	std::string							_storage;

	public:

		Server();
		~Server();
		Server(int port);
		void	setup_serv();
		void	run_serv();
		int		port_launch();

		void	accept_connections();
		void	squeeze_poll();
		void	check_values();

		void	setup_err(int err, const char *msg);
		void	handle_event(size_t ind);
		void	parse_first_line(std::string line);
		void	parse_header(char* line);
		void	squeeze_client_vect(int to_find);

		void	addToPollFds(std::vector<int>& vect_client, size_t old_size);
		bool	handle_existing_connection(struct pollfd *poll);

		int		recieve_data(struct pollfd	*ptr_tab_poll);
		int		send_response(struct pollfd *poll);
		int		get_server_fd(){ return(_serv_fd); }
		int		getPort(){ return _port; }
		bool	check_error(){ return _err; }

		std::vector<int>& get_clients(){ return _clients; }
		std::vector<int>& get_port_numbers(){ return _port_numbers; }

		void	set_port(int port){ _port = port; }
		void	displayAvailableServer();

		void	process_request();
		void	process_post_request();
		void	process_delete_request();
		void	process_get_request();

		void	store_data();
		bool	end_reached();
};

/* utils.cpp */
void *get_in_addr(struct sockaddr *sa);
std::map<std::string, std::string> initialize_mime_types(void);
std::map<std::string, std::string> http_table(void);

/* error_page.cpp */
std::string generate_html(const std::string &key);
std::string delete_page( void );
std::string basic_page( void );
std::string post_page( void );

/* handlers.cpp */

#endif
