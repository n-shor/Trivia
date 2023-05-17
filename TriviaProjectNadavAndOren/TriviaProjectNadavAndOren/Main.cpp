#include "Server.h"

int main() 
{
	Server::getInstance().run();
	Server::destroyInstance();
	return 0;
}