#pragma once
#include "IRequestHandler.h"
#include "LoginRequestHandler.hpp"
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <map>
#include <thread>
#include <exception>
#include <iostream>

class Communicator
{
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	void bindAndListen();
	void handleNewClient(SOCKET);
public:
	void startHandleRequests();
};