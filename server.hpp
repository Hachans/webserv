#ifndef server_hpp
#define server_hpp

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For read
#include <fcntl.h> // For fcntl
#include <netdb.h> // For addrinfo
#include <arpa/inet.h> // For inet_ntop
#include <stdio.h> // For fclose()
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <sstream> // For stringstream
#include <map>
#include <fstream>


#define PORT "4242"

/* utils.cpp */
void *get_in_addr(struct sockaddr *sa);
char* parse_method(char line[], const char symbol[]);
char* parse(char line[], const char symbol[]);
std::map<std::string, std::string> initialize_mime_types(void);
std::map<std::string, std::string> http_table(void);

/* error_page.cpp */
std::string generate_html(const std::string &key);
std::string delete_response( void );

/* handlers.cpp */
std::string valid_get(std::fstream &test, std::map<std::string, std::string> &map, std::string type);
std::string invalid_get(const std::string &err_cd, std::map<std::string, std::string> &map);
std::string valid_delete(std::string file_name, std::string type, std::map<std::string, std::string> &map);
std::string invalid_delete(const std::string &err_cd, std::map<std::string, std::string> &map);
std::string valid_post(std::string file_name, std::string type, std::map<std::string, std::string> &map);
std::string invalid_post(std::string err_cd, std::map<std::string, std::string> &map);

#endif
