#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode <= 1 || requestInfo.messageCode >= 0;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	RequestHandlerFactory::getInstance().getRoomManager().getRoom(m_room.getRoomData().id).removeUser(m_user.getUsername());

	RequestResult r;
	LeaveRoomResponse lrr;
	lrr.status = leaveRoomSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(lrr);
	return r;
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

RequestResult RoomMemberRequestHandler::getRoomsState(RequestInfo)
{
	try{
		if (!contains(m_room.getAllUsers(), m_room.getRoomData().adminName))
		{
			throw 69;
		}
		RequestResult r;
		GetRoomStateResponse grsr;
		grsr.answerTimeout = m_room.getRoomData().timePerQuestion;
		grsr.hasGameBegun = m_room.getRoomData().isActive == 1;
		grsr.questionCount = m_room.getRoomData().numOfQuestionsInGame;
		grsr.players = m_room.getAllUsers();
		grsr.status = getRoomsStateRes;
		if (m_room.getRoomData().isActive != 0)
		{
			r.newHandler = RequestHandlerFactory::getInstance().createGameRequestHandler(m_user, RequestHandlerFactory::getInstance().getGameManager().findUserGame(m_user.getUsername()));
		}
		else {
			r.newHandler = RequestHandlerFactory::getInstance().createRoomMemberRequestHandler(m_user, m_room);
		}
			r.response = JsonResponsePacketSerializer::serializeResponse(grsr);
		return r;
    }
	catch (...)
	{
		RequestResult r;
		ErrorResponse e;
		e.message = "room closed";
		r.response = JsonResponsePacketSerializer::serializeResponse(e);
		r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
		return r;
	}
}

RoomMemberRequestHandler::RoomMemberRequestHandler(std::string username, Room& room) : m_room(room), m_user(username)
{
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (isRequestRelevant(requestInfo))
	{
		switch (requestInfo.messageCode)
		{
		case LeaveRoom:
			return this->leaveRoom(requestInfo);
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
