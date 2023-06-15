#pragma once
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"

enum menuRequestTypes { CreateRoom, GetRooms, GetPlayersInRoom, JoinRoom, GetStatistics, Logout, GetHighScore};
enum status {signedOut, GetRoomsSuccessful, GetPlayersInRoomSuccessful, GetPersonalStatsSuccessful, GetHighScoreSuccessful, joinRoomSuccessful, joinRoomUnSuccessful, CreateRoomSuccessful};

class MenuRequestHandler : public IRequestHandler
{
private:
    LoggedUser m_user;
    RequestHandlerFactory& m_handlerFactory;

    bool isRequestRelevant(const RequestInfo& requestInfo) override;

    RequestResult signout(RequestInfo);
    RequestResult getRooms(RequestInfo);
    RequestResult getPlayersInRoom(RequestInfo);
    RequestResult getPersonalStats(RequestInfo);
    RequestResult getHighScore(RequestInfo);
    RequestResult joinRoom(RequestInfo);
    RequestResult createRoom(RequestInfo);

public:
    MenuRequestHandler(std::string username, RequestHandlerFactory& rhf, RoomManager& rm);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};
