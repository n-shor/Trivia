#pragma once
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"

enum menuRequestTypes { CreateRoom = 21, GetRooms, GetPlayersInRoom, JoinRoom, GetStatistics, Logout, GetHighScore, AddQuestion = 110 };
enum status { signedOut = 28, GetRoomsSuccessful, GetPlayersInRoomSuccessful, GetPersonalStatsSuccessful, GetHighScoreSuccessful, joinRoomSuccessful, joinRoomUnSuccessful, CreateRoomSuccessful, InvalidQuestion = 170 , QuestionAdded};

class MenuRequestHandler : public IRequestHandler
{
private:
    LoggedUser m_user;

    bool isRequestRelevant(const RequestInfo& requestInfo) override;

    RequestResult signout(RequestInfo);
    RequestResult getRooms(RequestInfo);
    RequestResult getPlayersInRoom(RequestInfo);
    RequestResult getPersonalStats(RequestInfo);
    RequestResult getHighScore(RequestInfo);
    RequestResult joinRoom(RequestInfo);
    RequestResult createRoom(RequestInfo);
    RequestResult addQuestion(RequestInfo);

public:
    MenuRequestHandler(std::string username);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};
