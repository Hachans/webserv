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

#define PORT		4242
#define SIZE_POLLFD	30000
#define BUFFER_SIZE	8192

class Server{
	int					_serv_fd;
	int					_listen_fd;
	char				_buffer[BUFFER_SIZE + 1];
	int					_port;
	bool				_end_connection;
	bool				_remove_client;
	bool				_remove_poll;
	bool				_err;
	std::vector<int>	_clients;
	struct sockaddr_in	_address;
	struct pollfd		_poll_fds[SIZE_POLLFD];
	nfds_t				_nfds;

	public:

		Server();
		~Server();
		Server(int port);
		void	setup_serv();
		void	run_serv();

		void	setup_err(int err, const char *msg);
		void	handle_event(size_t ind);
		void	squeeze_poll();

		std::vector<int>& get_clients(){ return(_clients); }
		int get_server_fd(){ return(_serv_fd); }

		void accept_connections();
		void addToPollFds(std::vector<int>& vect_client, size_t old_size);
		bool handle_existing_connection(struct pollfd *poll);

		int recieve_data(struct pollfd	*ptr_tab_poll);

};

/* utils.cpp */
void *get_in_addr(struct sockaddr *sa);
char* parse_method(char line[], const char symbol[]);
char* parse(char line[], const char symbol[]);
char* parse_version(char line[], const char symbol[]);
std::map<std::string, std::string> initialize_mime_types(void);
std::map<std::string, std::string> http_table(void);

/* error_page.cpp */
std::string generate_html(const std::string &key);
std::string delete_response( void );
std::string post_page( void );

/* handlers.cpp */


#endif
