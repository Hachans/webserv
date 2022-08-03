#ifndef CGI_hpp
# define CGI_hpp

#include "server.hpp"

#ifdef __linux__
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <string.h>
#endif

extern t_gconf *gconf;

class CGI
{
	private:
		std::map<std::string, std::string> _env;
		std::string _ret_code;
	public:
		CGI(std::map<std::string, std::string> const &env);
		~CGI();

		std::string const &execCGI(std::string const &filePath, const std::string &root);
};

#endif
