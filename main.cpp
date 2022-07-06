#include "server.hpp"

int main(int argc, char *argv[])
{
	if(argc != 2)
		std::cout << "Invalid arg count\n";
	else{
		Server sv(atoi(argv[1]));
		sv.setup_serv();
	}
	return (0);
}
