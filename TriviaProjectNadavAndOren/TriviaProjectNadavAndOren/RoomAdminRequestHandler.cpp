#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "GameRequestHandler.h"

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode == CloseRoom || requestInfo.messageCode == StartGame || requestInfo.messageCode == GetRoomsState;
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo)
{
	RequestHandlerFactory::getInstance().getRoomManager().deleteRoom(m_room.getRoomData().id);

	RequestResult r;
	CloseRoomResponse crr;
	crr.status = closeRoomSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(crr);
	return r;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	RequestResult r;
	StartGameResponse sgr;


	try {
		sgr.status = startGameSuccessful;
		m_room.activateRoom();
		r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, RequestHandlerFactory::getInstance().getGameManager().createGame(m_room));
		r.response = JsonResponsePacketSerializer::serializeResponse(sgr);
		return r;
	}
	catch (int sixtyNine)
	{
		if (sixtyNine == 69)
		{
			sgr.status = theServerDoesntHaveEnoughQuestions;
		}
		else {
			sgr.status = startGameUnsuccessful;
		}
		r.newHandler = RequestHandlerFactory::getInstance().createRoomAdminRequestHandler(m_user,
			RequestHandlerFactory::getInstance().getRoomManager().getRoom(m_room.getRoomData().id));
		r.response = JsonResponsePacketSerializer::serializeResponse(sgr);
		return r;
	}
}

template <typename T>
bool contains(std::vector<T> vec, const T& elem)
{
	bool result = false;
	if (find(vec.begin(), vec.end(), elem) != vec.end())
	{
		result = true;
	}
	return result;
}

RequestResult RoomAdminRequestHandler::getRoomsState(RequestInfo)
{
	try {
		if (!contains(m_room.getAllUsers(), m_room.getRoomData().adminName))
		{
			throw 68;
		}
		RequestResult r;
		GetRoomStateResponse grsr;
		grsr.answerTimeout = m_room.getRoomData().timePerQuestion;
		grsr.hasGameBegun = m_room.getRoomData().isActive;
		grsr.questionCount = m_room.getRoomData().numOfQuestionsInGame;
		grsr.players = m_room.getAllUsers();
		grsr.status = getRoomsStateRes;
		r.response = JsonResponsePacketSerializer::serializeResponse(grsr);
		r.newHandler = RequestHandlerFactory::getInstance().createRoomAdminRequestHandler(m_user,
			RequestHandlerFactory::getInstance().getRoomManager().getRoom(m_room.getRoomData().id));
		
		return r;
	}
	catch (int sixtyNine)
	{
		std::cout << sixtyNine << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
	catch(...)
	{

	}
	RequestResult r;
	ErrorResponse e;
	e.message = "room closed";
	r.response = JsonResponsePacketSerializer::serializeResponse(e);
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	return r;
}

RoomAdminRequestHandler::RoomAdminRequestHandler(std::string username, Room& room) : m_room(room), m_user(username)
{
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		switch (requestInfo.messageCode)
		{
		case CloseRoom:
			return this->closeRoom(requestInfo);
		case StartGame:
			return this->startGame(requestInfo);
		case GetRoomsState:
			return this->getRoomsState(requestInfo);
		}
	}
	RequestResult r;
	ErrorResponse e;
	e.message = "irrelevant message";
	r.response = JsonResponsePacketSerializer::serializeResponse(e);
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	return r;
}

