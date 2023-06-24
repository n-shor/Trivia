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
#include <memory>

class Communicator
{
private:
    SOCKET m_serverSocket;
    std::map<SOCKET, std::pair<std::string, std::unique_ptr<IRequestHandler>>> m_clients;
    void bindAndListen();
    void handleNewClient(SOCKET s);

    // Private constructor so that no objects can be created.
    Communicator() = default;

    // Private copy constructor and assignment operator to prevent copying
    Communicator(const Communicator&) = delete;
    Communicator& operator=(const Communicator&) = delete;

public:
    void startHandleRequests();

    static Communicator& getInstance() {
        static Communicator instance;
        return instance;
    }

};

