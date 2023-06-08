#include "Communicator.h"
#define PORT 8080
#define TEMP_STATUS_VAL 69

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

    int messageSize = (headerData[1] - '0') * 1000 + (headerData[2] - '0') * 100 + (headerData[3] - '0') * 10 + (headerData[4] - '0');
    
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

    bool continueReceiving = true;

    while (continueReceiving)
    {
        // Receives the JSON message from the client
        auto [messageCode, messageData] = recvMessage(s);
        messageCode -= '0'; //turning char into int

        std::cout << "Received message (type " << static_cast<int>(messageCode) << "): " << messageData << std::endl;

        if (messageData == "end")
        {
            std::cout << "End of client messages.\n";
            continueReceiving = false;
        }
        else
        {
            // Prepare the RequestInfo object
            RequestInfo ri;
            ri.messageCode = messageCode;
            ri.messageContent = std::vector<unsigned char>(messageData.begin(), messageData.end());

            // Process the received JSON message
            RequestResult reqRes = m_clients[s]->handleRequest(ri);
            std::vector<unsigned char> res = reqRes.response;
            m_clients[s] = reqRes.newHandler;
            std::string msg(res.begin(), res.end());
            std::cout << msg << std::endl;
            send(s, msg.c_str(), msg.size(), 0);
        }
    }

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
        RequestHandlerFactory rhf;
        m_clients[clientSocket] = new LoginRequestHandler(rhf);
        std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
    }
}
