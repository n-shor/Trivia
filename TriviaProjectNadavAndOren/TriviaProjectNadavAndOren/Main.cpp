#include "Server.h"

int main() 
{
	try
	{
		Server::getInstance().run();
		Server::destroyInstance();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}