#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

std::unique_ptr<RequestHandlerFactory> RequestHandlerFactory::instance;

RequestHandlerFactory::RequestHandlerFactory() : m_StatisticsManager(), m_gameManager(m_StatisticsManager.getDB())
{
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::make_unique<LoginRequestHandler>();
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(std::string username)
{
    return std::make_unique<MenuRequestHandler>(username);
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

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser lu, Room& r)
{
    return std::make_unique<RoomAdminRequestHandler>(lu.getUsername(), r);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser lu, Room& r)
{
    return std::make_unique<RoomMemberRequestHandler>(lu.getUsername(), r);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(LoggedUser lu, Game g)
{
    return std::make_unique<GameRequestHandler>(lu.getUsername(), g);
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return m_gameManager;
}
