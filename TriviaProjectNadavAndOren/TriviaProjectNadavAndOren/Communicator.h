#pragma once
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include <WinSock2.h>
#include <string>
#include <vector>
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