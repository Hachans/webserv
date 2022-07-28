#ifndef CGI_hpp
#define CGI_hpp

#include "server.hpp"

extern t_gconf *gconf;

class CGI
{
	private:
		std::map<std::string, std::string> _env;
	public:
		CGI(std::map<std::string, std::string> const &env);
		~CGI();

		int execCGI(std::string const &filePath);
};

#endif
