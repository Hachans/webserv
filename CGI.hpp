#ifndef CGI_hpp
#define CGI_hpp

#include "server.hpp"

extern t_gconf *gconf;

class CGI
{
	private:
		std::map<std::string, std::string> _env;
		std::string _ret_code;
	public:
		CGI(std::map<std::string, std::string> const &env);
		~CGI();

		std::string const &execCGI(std::string const &filePath);
};

#endif
