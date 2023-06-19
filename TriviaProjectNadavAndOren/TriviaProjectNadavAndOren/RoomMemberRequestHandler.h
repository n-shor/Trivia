#pragma once
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"

enum RoomMemberRequestTypes { LeaveRoom, GetRoomsState };
enum RoomMemberRequeststatus { leaveRoomSuccessful, getRoomsStateRes};

class RoomMemberRequestHandler : public IRequestHandler
{
private:

    Room m_room;
    RoomManager& m_roomManager;
    LoggedUser m_user;

    bool isRequestRelevant(const RequestInfo& requestInfo) override;

    RequestResult leaveRoom(RequestInfo);
    RequestResult getRoomsState(RequestInfo);

public:
    RoomMemberRequestHandler(std::string username, Room room);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};