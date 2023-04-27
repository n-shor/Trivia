#include "Communicator.h"
#define PORT 8080

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		// this accepts the client and create a specific socket from server to this client
		// the process will not continue until a client connects to the server
		SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "accepting client..." << '\n';
		// the function that handle the conversation with the client
		std::thread(&Communicator::handleNewClient, this, client_socket).detach();
	}

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

	// ff the received message is "Hello", sends "Hello" back
	if (std::string(buffer) == "Hello") {
		message = "Hello";
		send(s, message.c_str(), message.size(), 0);
	}

	// closes the client socket
	closesocket(s);
}




