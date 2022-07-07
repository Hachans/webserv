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
#include <fstream>
#include <ctime>

#define PORT		4242
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
	nfds_t								_nfds;
	std::map<std::string, std::string>	_http_request;
	std::map<std::string, std::string>	_response;
	std::map<std::string, std::string>	_http_table;
	std::map<std::string, std::string>	_mime_types;
	size_t								_file_size;

	public:

		Server();
		~Server();
		Server(int port);
		void	setup_serv();
		void	run_serv();

		void	accept_connections();
		void	squeeze_poll();
		void	check_values();

		void	setup_err(int err, const char *msg);
		void	handle_event(size_t ind);
		void	parse_first_line(std::string line);

		void	addToPollFds(std::vector<int>& vect_client, size_t old_size);
		bool	handle_existing_connection(struct pollfd *poll);

		int		recieve_data(struct pollfd	*ptr_tab_poll);
		int		send_response(struct pollfd *poll);
		int		get_server_fd(){ return(_serv_fd); }

		std::vector<int>& get_clients(){ return(_clients); }

		void	create_get_response();

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
