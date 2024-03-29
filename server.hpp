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
#include <sys/stat.h>
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <iterator>
#include <ctime>
#include <dirent.h>

#include "config.hpp"
#include "defines.hpp"

class conf_data;

class Server{
	int									_serv_fd; //
	int									_listen_fd;//
	int									_port;//
	std::vector<int>					_clients;//
	struct sockaddr_in					_address;//
	char								_buffer[BUFFER_SIZE + 1];
	std::string							_storage;
	bool								_end_connection;
	std::map<std::string, std::string>	_http_request;
	std::string							_storage_data;
	bool								_finished;
	bool								_remove_client; //?

	bool								_err;
	std::map<std::string, std::string>	_response;
	std::map<std::string, std::string>	_http_table;
	std::map<std::string, std::string>	_mime_types;
	std::vector<std::string>	 		_cgi_types;
	int									_file_size;
	int									_file_offset;
	std::string							_err_string;
	std::string							_cgi_response;
	std::vector<conf_data*>				_data_vec;
	conf_data							*_data;
	bool								_dir;
	bool								_is_cgi;
	bool								_cgi_err;
	


	public:

		Server(int port);
		Server();
		Server(conf_data *data);
		~Server();
		void	setup_serv();

		void	accept_connections();
		void	check_values();
		void	storePostData();

		std::string	redir_body(std::string url);

		void	setup_err(int err, const char *msg);
		void	parse_first_line(std::string line);
		void	parse_header(char* line);
		void	squeeze_client_vect(int to_find);
		void	setBodyGet(std::string err_str);

		bool	handle_existing_connection(struct pollfd *poll);

		int		recieve_data(struct pollfd	*ptr_tab_poll);
		int		send_response(struct pollfd *poll);
		int		get_server_fd(){ return(_serv_fd); }
		int		getPort(){ return _port; }
		bool	check_error(){ return _err; }

		std::vector<int>& get_clients(){ return _clients; }

		void	set_port(int port){ _port = port; }

		void	process_request();
		void	process_post_request();
		void	process_delete_request();
		void	process_get_request();

		void	store_data();
		bool	end_reached();
		conf_data *get_data(){ return _data; }

		void	displayFiles();

		void	addData(conf_data *d){
			_data_vec.push_back(d);
		}

		std::map<std::string, std::string> getCgiEnv(void);

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
