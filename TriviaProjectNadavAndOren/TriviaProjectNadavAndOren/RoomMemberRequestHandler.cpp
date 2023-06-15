#include "RoomMemberRequestHandler.h"

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode <= 1 || requestInfo.messageCode >= 0;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	m_roomManager.getRoom(m_room.getRoomData().id).removeUser(m_user.getUsername());

	RequestResult r;
	LeaveRoomResponse lrr;
	lrr.status = leaveRoomSuccessful;
	r.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(lrr);
	return r;
}

RequestResult RoomMemberRequestHandler::getRoomsState(RequestInfo)
{
	try{
		RequestResult r;
		GetRoomStateResponse grsr;
		grsr.answerTimeout = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().timePerQuestion;
		grsr.hasGameBegun = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().isActive;
		grsr.questionCount = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().numOfQuestionsInGame;
		grsr.players = m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers();
		grsr.status = getRoomsStateRes;
		if (m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().isActive != 0)
		{
			//!!!!!
			//!!!!!
			r.newHandler = nullptr; //later this will point to handler for game
			//!!!!!
			//!!!!!

		}
		else {
			r.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, m_roomManager.getRoom(m_room.getRoomData().id));
		}
			r.response = JsonResponsePacketSerializer::serializeResponse(grsr);
		return r;
    }
	catch (...) //we should check if this causes problems with the sudden disconnects later, or just check what error we should be catching
	{
		RequestResult r;
		ErrorResponse e;
		e.message = "room closed";
		r.response = JsonResponsePacketSerializer::serializeResponse(e);
		r.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
		return r;
	}
}

RoomMemberRequestHandler::RoomMemberRequestHandler(std::string username, RequestHandlerFactory& rhf, Room room) : m_room(room), m_user(username), m_handlerFactory(rhf), m_roomManager(rhf.getRoomManager())
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
	r.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
	return r;
}
