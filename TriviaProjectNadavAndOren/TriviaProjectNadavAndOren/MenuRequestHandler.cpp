#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

MenuRequestHandler::MenuRequestHandler(std::string username) : m_user(username)
{
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	switch (requestInfo.messageCode)
	{
	case CreateRoom:
		return createRoom(requestInfo);
	case GetRooms:
		return getRooms(requestInfo);
	case GetPlayersInRoom:
		return getPlayersInRoom(requestInfo);
	case JoinRoom:
		return joinRoom(requestInfo);
	case GetStatistics:
		return getPersonalStats(requestInfo);
	case Logout:
		RequestHandlerFactory::getInstance().getLoginManager().logout(m_user.getUsername());
		return signout(requestInfo);
	case GetHighScore:
		return getHighScore(requestInfo);
	}

	RequestResult r;
	r.username = m_user.getUsername();
	ErrorResponse e;
	e.message = "irrelevant message";
	r.response = JsonResponsePacketSerializer::serializeResponse(e);
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	return r;
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.messageCode == CreateRoom || requestInfo.messageCode == GetRooms || requestInfo.messageCode == GetPlayersInRoom ||
		requestInfo.messageCode == JoinRoom || requestInfo.messageCode == GetStatistics || requestInfo.messageCode == Logout || requestInfo.messageCode == GetHighScore;
}

RequestResult MenuRequestHandler::signout(RequestInfo)
{
	RequestResult r;
	r.username = m_user.getUsername();
	LogoutResponse lr;
	lr.status = signedOut;
	r.newHandler = RequestHandlerFactory::getInstance().createLoginRequestHandler();
	r.response = JsonResponsePacketSerializer::serializeResponse(lr);
	return r;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo)
{
	RequestResult r;
	r.username = m_user.getUsername();
	GetRoomsResponse grr;
	grr.rooms = RequestHandlerFactory::getInstance().getRoomManager().getRooms();
	grr.status = GetRoomsSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	return r;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo ri)
{
	GetPlayersInRoomRequest gpir = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(ri);
	RequestResult r;
	r.username = m_user.getUsername();
	GetPlayesInRoomResponse grr;
	grr.players = RequestHandlerFactory::getInstance().getRoomManager().getRoom(gpir.roomId).getAllUsers();
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	return r;
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo)
{
	RequestResult r;
	r.username = m_user.getUsername();
	getPersonalStatsResponse grr;
	grr.statistics = RequestHandlerFactory::getInstance().getStatisticsManager().getUserStatistics(m_user.getUsername());
	grr.status = GetPersonalStatsSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	return r;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo)
{
	RequestResult r;
	r.username = m_user.getUsername();
	getHighScoreResponse grr;
	grr.statistics = RequestHandlerFactory::getInstance().getStatisticsManager().getHighScore();
	grr.status = GetHighScoreSuccessful;
	r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
	r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	return r;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo ri)
{
	JoinRoomRequest gpir = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(ri);
	RequestResult r;
	r.username = m_user.getUsername();
	JoinRoomResponse grr;
	if (RequestHandlerFactory::getInstance().getRoomManager().getRoomState(gpir.roomId) == isActive ||
		RequestHandlerFactory::getInstance().getRoomManager().getRoom(gpir.roomId).getRoomData().currentPlayers == 
		RequestHandlerFactory::getInstance().getRoomManager().getRoom(gpir.roomId).getRoomData().maxPlayers) //checking if the room is active or full
	{
		r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(m_user.getUsername());
		grr.status = joinRoomUnSuccessful;
		r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	}

	else
	{
		std::lock_guard<std::mutex> lock(RequestHandlerFactory::getInstance().getRoomManager().m_roomsMutex);

		RequestHandlerFactory::getInstance().getRoomManager().getRoom(gpir.roomId).addUser(m_user);
		grr.status = joinRoomSuccessful;
		r.newHandler = RequestHandlerFactory::getInstance().createRoomMemberRequestHandler(m_user, RequestHandlerFactory::getInstance().getRoomManager().getRoom(gpir.roomId));
		r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	}

	return r;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo ri)
{
	CreateRoomRequest gpir = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(ri);
	RequestResult r;
	r.username = m_user.getUsername();
	CreateRoomResponse grr;

	RoomData rd;
	rd.isActive = isntActive;
	rd.currentPlayers = 0; //we will add the creator later and then it'll be 1
	rd.maxPlayers = gpir.maxUsers;
	rd.name = gpir.roomName;
	rd.adminName = m_user.getUsername();
	rd.numOfQuestionsInGame = gpir.questionCount;
	rd.timePerQuestion = gpir.answerTimeout;
	RequestHandlerFactory::getInstance().getRoomManager().currId++;
	rd.id = RequestHandlerFactory::getInstance().getRoomManager().currId;

	RequestHandlerFactory::getInstance().getRoomManager().createRoom(rd, m_user);
	grr.status = CreateRoomSuccessful;
	grr.roomId = rd.id;
	grr.adminName = m_user.getUsername();
	r.newHandler = RequestHandlerFactory::getInstance().createRoomAdminRequestHandler(m_user, RequestHandlerFactory::getInstance().getRoomManager().getRoom(rd.id));
	r.response = JsonResponsePacketSerializer::serializeResponse(grr);
	return r;
}
