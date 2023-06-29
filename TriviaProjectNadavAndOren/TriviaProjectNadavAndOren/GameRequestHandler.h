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
	Game& m_game;
	LoggedUser m_user;
	RequestResult getQuestion(const RequestInfo) const;
	RequestResult submitAnswer(const RequestInfo) const;
	RequestResult getGameResults(const RequestInfo) const;
	RequestResult leaveGame(const RequestInfo) const;
	RequestResult leaderboard(const RequestInfo) const;
	RequestResult checkGameEnd(const RequestInfo) const;
	bool isRequestRelevant(const RequestInfo& requestInfo) const override;
	static bool m_gameEnded;

public:
	GameRequestHandler(std::string, Game&);
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
};