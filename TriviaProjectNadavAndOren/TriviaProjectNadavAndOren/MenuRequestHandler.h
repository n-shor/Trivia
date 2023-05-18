#pragma once
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

enum menuRequestTypes { CreateRoomRequest, GetRoomsRequest, GetPlayersInRoomRequest, JoinRoomRequest, GetStatisticsRequest, LogoutRequest};

class MenuRequestHandler : public IRequestHandler
{
private:
    LoggedUser m_user;
    RoomManager& m_roomManager;
    //needs statistics manager
    RequestHandlerFactory& m_handlerFactory;

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

    RequestResult signout(RequestInfo);
    RequestResult getRooms(RequestInfo);
    RequestResult getPlayersInRoom(RequestInfo);
    RequestResult getPersonalStats(RequestInfo);
    RequestResult getHighScore(RequestInfo);
    RequestResult joinRoom(RequestInfo);
    RequestResult createRoom(RequestInfo);

public:
    MenuRequestHandler(std::string username, RequestHandlerFactory& rhf, RoomManager& rm);
    //you forgot to put refernces (&) here
    // continue here 
};
