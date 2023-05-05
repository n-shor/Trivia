#pragma once
#include "IRequestHandler.h"
#include "RegisterRequestHandler.hpp"
#include "LoginRequestHandler.hpp"
#include "JsonResponsePacketSerializer.hpp"
#include <WinSock2.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <exception>
#include <iostream>
#include "ErrorResponseHandler.hpp"

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