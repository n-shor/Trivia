#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode >= 0 && requestInfo.messageCode <= 2;
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo)
{
	m_roomManager.deleteRoom(m_room.getRoomData().id);

	RequestResult r;
	CloseRoomResponse crr;
	crr.status = closeRoomSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(crr);
	return r;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	m_room =  Room(m_roomManager.getRoom(m_room.getRoomData().id).getRoomData());
	for (int i = 0; i < m_room.getRoomData().currentPlayers; i++)
		m_room.addUser(m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers()[i]);

	m_roomManager.deleteRoom(m_room.getRoomData().id);
	m_roomManager.createRoom(m_room.getRoomData(), m_user);

	for (int i = 0; i < m_room.getRoomData().currentPlayers; i++)
		m_roomManager.getRoom(m_room.getRoomData().id).addUser(m_room.getAllUsers()[i]);
	

	RequestResult r;
	StartGameResponse sgr;
	sgr.status = startGameSuccessful;


	//!!!!!!!
	//!!!!!!!
	r.newHandler = nullptr; //for now its null, later when we have a game handler, we will put it here
	//!!!!!!!
	//!!!!!!!
	
	r.response = JsonResponsePacketSerializer::serializeResponse(sgr);
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

RequestResult RoomAdminRequestHandler::getRoomsState(RequestInfo)
{
	try {
		if (!contains(m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers(), m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().adminName))
		{
			throw 69;
		}
		RequestResult r;
		GetRoomStateResponse grsr;
		grsr.answerTimeout = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().timePerQuestion;
		grsr.hasGameBegun = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().isActive;
		grsr.questionCount = m_roomManager.getRoom(m_room.getRoomData().id).getRoomData().numOfQuestionsInGame;
		grsr.players = m_roomManager.getRoom(m_room.getRoomData().id).getAllUsers();
		grsr.status = getRoomsStateRes;
		r.response = JsonResponsePacketSerializer::serializeResponse(grsr);
		r.newHandler = RequestHandlerFactory::getInstance().createRoomAdminRequestHandler(m_user, m_roomManager.getRoom(m_room.getRoomData().id));
		
		return r;
	}
	catch(...) //we should check if this causes problems with the sudden disconnects later, or just check what error we should be catching
	{
		RequestResult r;
		ErrorResponse e;
		e.message = "room closed";
		r.response = JsonResponsePacketSerializer::serializeResponse(e);
		r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
		return r;
	}
}

RoomAdminRequestHandler::RoomAdminRequestHandler(std::string username, Room room) : m_room(room), m_user(username), m_roomManager(RequestHandlerFactory::getInstance().getRoomManager())
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

