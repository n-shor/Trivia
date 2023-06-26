#pragma once
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RoomMemberRequestHandler.h"

enum RoomAdminRequestTypes { CloseRoom = 43, StartGame = 2 };
enum RoomAdminRequeststatus { closeRoomSuccessful = 13, startGameSuccessful, startGameUnsuccessful, theServerDoesntHaveEnoughQuestions };

class RoomAdminRequestHandler : public IRequestHandler
{
private:
    
    Room& m_room;
    LoggedUser m_user;

    bool isRequestRelevant(const RequestInfo& requestInfo) override;

    RequestResult closeRoom(RequestInfo);
    RequestResult startGame(RequestInfo);
    RequestResult getRoomsState(RequestInfo);

public:
    RoomAdminRequestHandler(std::string username, Room& room);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};
