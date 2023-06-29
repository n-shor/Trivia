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

    bool isRequestRelevant(const RequestInfo& requestInfo) const override;

    RequestResult signout(const RequestInfo);
    RequestResult getRooms(const RequestInfo);
    RequestResult getPlayersInRoom(const RequestInfo);
    RequestResult getPersonalStats(const RequestInfo);
    RequestResult getHighScore(const RequestInfo);
    RequestResult joinRoom(const RequestInfo);
    RequestResult createRoom(const RequestInfo);
    RequestResult addQuestion(const RequestInfo);

public:
    MenuRequestHandler(std::string username);
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};
