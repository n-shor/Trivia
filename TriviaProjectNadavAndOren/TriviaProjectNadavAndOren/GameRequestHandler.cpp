#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Game.h"

bool GameRequestHandler::m_gameEnded = false;

RequestResult GameRequestHandler::getQuestion(const RequestInfo ri)
{
	RequestResult r;
	r.username = m_user.getUsername();
	GetQuestionResponse gqr;
	gqr.status = getQuestionSuccessful;
	gqr.question = m_game.getQuestionForUser(m_user.getUsername()).getQuestion();
	std::map <unsigned int, std::string> ans;
	for (int i = 0; i < 4; i++) {
		ans[i] = m_game.getQuestionForUser(m_user.getUsername()).getPossibleAnswers()[i];
	}
	gqr.answers = ans;
	r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, m_game);
	r.response = JsonResponsePacketSerializer::serializeResponse(gqr);
	return r;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo ri)
{
	SubmitAnswerRequest sart = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(ri);
	RequestResult r;
	r.username = m_user.getUsername();
	SubmitAnswerResponse sare;
	sare.status = m_game.submitAnswer(m_user.getUsername(), sart.answerId);
	if (sare.status == Game::playerFinished && m_game.hasGameEnded())
	{
		m_gameEnded = true;
	}
	sare.correctAnswerId = m_game.getCorrectAnswerId(m_user.getUsername());
	r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, m_game);
	r.response = JsonResponsePacketSerializer::serializeResponse(sare);
	return r;
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo ri)
{
	RequestResult r;
	r.username = m_user.getUsername();
	GetGameResultsResponse ggrr;
	ggrr.status = gameEnded;
	std::vector<PlayerResults> prs;
	for (auto it = m_game.getPlayers().begin(); it != m_game.getPlayers().end(); it++)
	{
		prs.push_back(PlayerResults(it->first, it->second.correctAnswerCount, it->second.wrongAnswerCount, it->second.AverageAnswerTime));
	}

	ggrr.results = prs;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(ggrr);
	return r;
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo)
{
	m_game.removePlayer(m_user.getUsername());

	RequestResult r;
	r.username = m_user.getUsername();
	LeaveGameResponse lgr;
	lgr.status = leaveGameResponse;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(lgr);
	return r;
}

RequestResult GameRequestHandler::leaderboard(const RequestInfo)
{
	RequestResult r;
	r.username = m_user.getUsername();
	leaderBoardResponse lbr;
	for (auto it = m_game.getPlayers().begin(); it != m_game.getPlayers().end();it++)
	{
		lbr.players[it->first] = ((it->second.correctAnswerCount * (1/ std::sqrt(it->second.AverageAnswerTime)) * 1000) / (it->second.correctAnswerCount + it->second.wrongAnswerCount)); //score calculation
	}
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(lbr);
	return r;
}

RequestResult GameRequestHandler::checkGameEnd(const RequestInfo)
{
	RequestResult r;
	CheckForEndReponse cfer;
	if (m_gameEnded)
	{
		cfer.gameEnded = 1;
	}
	else
	{
		cfer.gameEnded = 0;
	}
	r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, m_game);
	r.response = JsonResponsePacketSerializer::serializeResponse(cfer);
	return r;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		switch (requestInfo.messageCode)
		{
		case getQuestionReq:
			return this->getQuestion(requestInfo);
		case submitAnswerReq:
			return this->submitAnswer(requestInfo);
		case getGameResultsReq:
			return this->getGameResults(requestInfo);
		case leaveGameReq:
			return this->leaveGame(requestInfo);
		case LeaderBoard:
			return this->leaderboard(requestInfo);
		case checkForEnd:
			return this->checkGameEnd(requestInfo);
		}
	}
	RequestResult r;
	r.username = m_user.getUsername();
	ErrorResponse e;
	e.message = "irrelevant message";
	r.response = JsonResponsePacketSerializer::serializeResponse(e);
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	return r;
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode == getQuestionReq || requestInfo.messageCode == submitAnswerReq ||
		requestInfo.messageCode == getGameResultsReq || requestInfo.messageCode == leaveGameReq || 
		requestInfo.messageCode == LeaderBoard || requestInfo.messageCode == checkForEnd;
}

GameRequestHandler::GameRequestHandler(std::string lu, Game& g) : m_game(g), m_user(lu)
{
}
