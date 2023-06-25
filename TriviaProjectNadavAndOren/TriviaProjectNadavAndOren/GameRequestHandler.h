#pragma once
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RoomMemberRequestHandler.h"

enum GameRequestTypes { getQuestionReq, submitAnswerReq, getGameResultsReq, leaveGameReq};
enum GameRequeststatus { getQuestionSuccessful, gameEnded, leaveGameResponse};

class GameRequestHandler : public IRequestHandler
{
private:
	bool m_cheatDetection;
	Game& m_game;
	LoggedUser m_user;
	RequestResult getQuestion(RequestInfo);
	RequestResult submitAnswer(RequestInfo);
	RequestResult getGameResults(RequestInfo);
	RequestResult leaveGame(RequestInfo);
	bool isRequestRelevant(const RequestInfo& requestInfo) override;
public:
	GameRequestHandler(std::string, Game&);
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
};