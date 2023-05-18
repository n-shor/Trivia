#include "MenuRequestHandler.h"

// TODO: Implement the members and methods for this class

MenuRequestHandler::MenuRequestHandler(std::string username, RequestHandlerFactory& rhf, RoomManager& rm) : m_user(username), m_roomManager(rm), m_handlerFactory(rhf)
{
	m_user = LoggedUser(username);
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode >= 0 && requestInfo.messageCode <= 5;
}
