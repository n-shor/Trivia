#include "Server.h"
#include "Communicator.h"
#include <iostream>

Server* Server::m_instancePtr = nullptr;

Server& Server::getInstance()
{
	if (m_instancePtr == nullptr)
	{
		m_instancePtr = new Server();
	}

	return *m_instancePtr;
}

void Server::destroyInstance()
{
	delete m_instancePtr;
	m_instancePtr = nullptr;
}

Server::Server() :
	m_communicator(),
	m_wsaData()
{	
	// initialize Winsock
	int result = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (result != 0)
	{
		std::cerr << "WSAStartup failed: " << result << std::endl;
		throw std::exception("WSAStartup failed");
	}

	// create a Communicator object
	m_communicator = Communicator();
}

Server::~Server()
{
	// cleanup Winsock
	WSACleanup();
}

void Server::run()
{
	try
	{
		// start handling requests
		m_communicator.startHandleRequests();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
