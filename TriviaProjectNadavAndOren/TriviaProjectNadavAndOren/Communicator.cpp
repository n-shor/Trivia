#include "Communicator.h"
#define PORT 8080

void Communicator::bindAndListen()
{
    // bind the server socket to a port
    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_serverSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        closesocket(m_serverSocket);
        throw std::runtime_error("Failed to bind socket: " + std::to_string(WSAGetLastError()));
    }

    // start listening for incoming connections
    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(m_serverSocket);
        throw std::runtime_error("Failed to listen on socket: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Listening on port " << PORT << std::endl;
}

void Communicator::handleNewClient(SOCKET s)
{
    std::cout << "Client connected." << std::endl;

    // sends "Hello" message to the client
    std::string message = "Hello";
    send(s, message.c_str(), message.size(), 0);

    // receives the "Hello" message from the client
    char buffer[6];
    int bytes_read = recv(s, buffer, 5, 0);
    buffer[bytes_read] = '\0';
    std::cout << "Received message from client: " << buffer << std::endl;

    // if the received message is "Hello", sends "Hello" back
    if (std::string(buffer) == "Hello") {
        message = "Hello";
        send(s, message.c_str(), message.size(), 0);
    }

    // closes the client socket
    closesocket(s);
}

void Communicator::startHandleRequests()
{
    // create the server socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Failed to create socket: " + std::to_string(WSAGetLastError()));
    }

    // bind the server socket to a port and start listening for incoming connections
    bindAndListen();

    // wait for incoming connections
    while (true)
    {
        std::cout << "Waiting for client connection request" << std::endl;

        // accept a new connection
        SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            closesocket(m_serverSocket);
            throw std::runtime_error("Failed to accept client connection: " + std::to_string(WSAGetLastError()));
        }

        std::cout << "Accepted client connection" << std::endl;

        // handle the client connection in a separate thread
        std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
    }
}

