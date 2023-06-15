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
    LoginRequestHandler* createLoginRequestHandler();
    MenuRequestHandler* createMenuRequestHandler(std::string username);
    LoginManager& getLoginManager();
    StatisticsManager& getStatisticsManager();
    RoomManager& getRoomManager();
    RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser, Room);
    RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser, Room);
};
