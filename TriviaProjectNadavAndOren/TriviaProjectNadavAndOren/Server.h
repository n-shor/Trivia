#pragma once
#include "Communicator.h"

class Server {
private:
	Communicator m_communicator;
	WSADATA m_wsaData;
	static Server* m_instancePtr;

	Server(); //the constructor is private to prevent direct instantiation
	Server(const Server&) = delete; //deleting copy constructor - preventing copies of the singleton instance from being created.
	Server(Server&&) = delete; //deleting move constructor
	Server& operator=(const Server&) = delete; //deleting assignment operator
	Server& operator=(Server&&) = delete; //deleting move assignment operator

public:
	~Server();
	void run();
	static Server& getInstance();
	static void destroyInstance();
};

