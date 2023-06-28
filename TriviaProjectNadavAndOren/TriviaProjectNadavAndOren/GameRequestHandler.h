#pragma once
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RoomMemberRequestHandler.h"

enum GameRequestTypes { getQuestionReq = 4, submitAnswerReq, getGameResultsReq, leaveGameReq, LeaderBoard = 240, checkForEnd = 255};
enum GameRequeststatus { getQuestionSuccessful = 8, gameEnded, leaveGameResponse};

class GameRequestHandler : public IRequestHandler
{
private:
	Game m_game;
	LoggedUser m_user;
	bool m_gameEnded = false;
	RequestResult getQuestion(RequestInfo);
	RequestResult submitAnswer(RequestInfo);
	RequestResult getGameResults(RequestInfo);
	RequestResult leaveGame(RequestInfo);
	RequestResult leaderboard(RequestInfo);
	RequestResult checkGameEnd(RequestInfo);
	bool isRequestRelevant(const RequestInfo& requestInfo) override;
public:
	GameRequestHandler(std::string, Game&);
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
};