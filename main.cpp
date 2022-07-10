#include "server.hpp"
#include "socket.hpp"

int main(int argc, char *argv[])
{
	std::vector<conf_data*> *co;
	t_gconf *gconf = new t_gconf;
	gconf->error_pages = new std::map<size_t, std::string>();
	gconf->CGI = new std::map<std::string, std::string>();

	try
	{
		if (argc == 2)
			co = readConfFile(gconf, argv[1]);
		else
			throw std::invalid_argument("Invalid argument");
		validate(co, gconf);
	}
	catch(const std::exception& e)
	{
		Color::Modifier f_red(Color::Red);
		Color::Modifier reset(Color::White, 0, 1);
		std::cerr << f_red << "\nError: " << e.what() << '\n';
		std::cerr << "Initiating with default settings" << reset <<"\n\n" ;
		gconf->error_pages->clear();
		gconf->CGI->clear();
		co = readConfFile(gconf, "conf/default.conf");
	}

	Socket sv(co);

	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ++ite)
		delete *ite;

	delete co;
	delete gconf->error_pages;
	delete gconf->CGI;
	delete gconf;
	return (0);
}
