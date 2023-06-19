#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory() : m_StatisticsManager()
{
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(std::string username)
{
    return new MenuRequestHandler(username, *this, this->m_roomManager);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_StatisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser lu, Room r)
{
    return new RoomAdminRequestHandler(lu.getUsername(), *this, r);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser lu, Room r)
{
    return new RoomMemberRequestHandler(lu.getUsername(), *this, r);
}
