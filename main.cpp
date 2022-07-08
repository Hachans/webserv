#include "server.hpp"

int main(int argc, char *argv[])
{
	if(argc > 2)
		std::cout << "Invalid arg count\n";
	else if (argc == 2)
		Server sv(atoi(argv[1]));
	else
		Server sv;
	return (0);
}
