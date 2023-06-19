#pragma once
#include "RoomManager.h"
#include "StatisticsManager.hpp"
#include "LoginManager.h"
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
    static std::unique_ptr<RequestHandlerFactory> instance;

protected:
    StatisticsManager m_StatisticsManager;
    RoomManager m_roomManager;
    LoginManager m_loginManager;

    RequestHandlerFactory();

public:
    static RequestHandlerFactory& getInstance()
    {
        if (!instance)
        {
            instance.reset(new RequestHandlerFactory());
        }
        return *instance;
    }

    RequestHandlerFactory(const RequestHandlerFactory&) = delete;
    RequestHandlerFactory& operator=(const RequestHandlerFactory&) = delete;

    std::unique_ptr<IRequestHandler> createLoginRequestHandler();
    std::unique_ptr<IRequestHandler> createMenuRequestHandler(std::string username);
    LoginManager& getLoginManager();
    StatisticsManager& getStatisticsManager();
    RoomManager& getRoomManager();
    std::unique_ptr<IRequestHandler> createRoomAdminRequestHandler(LoggedUser, Room);
    std::unique_ptr<IRequestHandler> createRoomMemberRequestHandler(LoggedUser, Room);
};
