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

//helper function in order to parse the message properly
std::pair<int, std::string> recvMessage(int clientSocket) {
    char headerData[5];
    recv(clientSocket, headerData, 5, 0);

    int messageType = headerData[0];
    int messageSize;
    memcpy(&messageSize, &headerData[1], 4);
    messageSize = ntohl(messageSize);

    std::vector<char> messageJson(messageSize);
    int bytesToReceive = messageSize;
    int bytesReceived = 0;

    while (bytesReceived < messageSize) {
        int received = recv(clientSocket, &messageJson[bytesReceived], bytesToReceive, 0);
        bytesReceived += received;
        bytesToReceive -= received;
    }

    std::string messageData(messageJson.begin(), messageJson.end());

    return { messageType, messageData };
}

void Communicator::handleNewClient(SOCKET s)
{
    std::cout << "Client connected." << std::endl;

    // Receives the JSON message from the client
    auto [messageCode, messageData] = recvMessage(s);
    std::cout << "Received message (type " << messageCode << "): " << messageData << std::endl;

    // Prepare the RequestInfo object
    RequestInfo ri;
    ri.messageCode = messageCode;
    ri.messageContent = std::vector<unsigned char>(messageData.begin(), messageData.end());

    // Process the received JSON message
    m_clients[s]->handleRequest(ri);

    // Closes the client socket
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
        m_clients[clientSocket] = new LoginRequestHandler();
        std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
    }
}

