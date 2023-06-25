#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"

RequestResult GameRequestHandler::getQuestion(RequestInfo ri)
{
	RequestResult r;
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

RequestResult GameRequestHandler::submitAnswer(RequestInfo ri)
{
	SubmitAnswerRequest sart = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(ri);
	RequestResult r;
	SubmitAnswerResponse sare;
	sare.status = m_game.submitAnswer(m_user.getUsername(), sart.answerId);
	sare.correctAnswerId = m_game.getCorrectAnswerId(m_user.getUsername());
	r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, m_game);
	r.response = JsonResponsePacketSerializer::serializeResponse(sare);
	return r;
}

RequestResult GameRequestHandler::getGameResults(RequestInfo ri)
{
	RequestResult r;
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

RequestResult GameRequestHandler::leaveGame(RequestInfo)
{
	m_game.removePlayer(m_user.getUsername());

	RequestResult r;
	LeaveGameResponse lgr;
	lgr.status = leaveGameResponse;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(lgr);
	return r;
}


RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		switch (requestInfo.messageCode)
		{
		case getQuestionReq:
			m_cheatDetection = true;
			return this->getQuestion(requestInfo);
		case submitAnswerReq:
			if (m_cheatDetection)
			{
				m_cheatDetection = false;
				return this->submitAnswer(requestInfo);
			}
			else {
				RequestResult r;
				ErrorResponse e;
				e.message = "irrelevant message";
				r.response = JsonResponsePacketSerializer::serializeResponse(e);
				r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
				return r;
			}
		case getGameResultsReq:
			return this->getGameResults(requestInfo);
		case leaveGameReq:
			return this->leaveGame(requestInfo);
		}
	}
	RequestResult r;
	ErrorResponse e;
	e.message = "irrelevant message";
	r.response = JsonResponsePacketSerializer::serializeResponse(e);
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	return r;
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode >= 0 && requestInfo.messageCode <= 3;
}

GameRequestHandler::GameRequestHandler(std::string lu, Game& g) : m_game(g), m_user(lu)
{
	m_cheatDetection = false;
}
