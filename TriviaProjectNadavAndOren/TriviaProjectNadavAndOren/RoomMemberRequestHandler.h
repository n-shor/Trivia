#pragma once
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"

enum RoomMemberRequestTypes { LeaveRoom = 17, GetRoomsState };
enum RoomMemberRequeststatus { leaveRoomSuccessful = 19, getRoomsStateRes };

class RoomMemberRequestHandler : public IRequestHandler
{
private:

    Room& m_room;
    LoggedUser m_user;

    bool isRequestRelevant(const RequestInfo& requestInfo) const override;

    RequestResult leaveRoom(RequestInfo);
    RequestResult getRoomsState(RequestInfo) const;

public:
    RoomMemberRequestHandler(std::string username, Room& room);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};