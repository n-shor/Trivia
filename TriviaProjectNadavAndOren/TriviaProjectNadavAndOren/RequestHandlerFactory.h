#pragma once
#include "RoomManager.h"
#include "StatisticsManager.hpp"
#include "LoginManager.h"
class LoginRequestHandler; // Forward declarations to avoid circular dependency problems
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
protected:
    StatisticsManager m_StatisticsManager;
    RoomManager m_roomManager;
    LoginManager m_loginManager;
public:
    RequestHandlerFactory();
    IRequestHandler* createLoginRequestHandler();
    IRequestHandler* createMenuRequestHandler(std::string username);
    LoginManager& getLoginManager();
    StatisticsManager& getStatisticsManager();
    RoomManager& getRoomManager();
    IRequestHandler* createRoomAdminRequestHandler(LoggedUser, Room);
    IRequestHandler* createRoomMemberRequestHandler(LoggedUser, Room);
};
