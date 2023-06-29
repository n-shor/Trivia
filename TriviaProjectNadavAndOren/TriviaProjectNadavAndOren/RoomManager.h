#pragma once
#include "Room.h"
#include <map>

class RoomManager
{
private:
	static std::map<unsigned int, Room> m_rooms;

public:
	void createRoom(RoomData& rd, LoggedUser lu);
	void deleteRoom(int ID);
	const unsigned int getRoomState(int ID) const;
	const std::vector<RoomData> getRooms() const;
	Room& getRoom(int ID) const;
	static std::mutex m_roomsMutex;
	static int currId;
};