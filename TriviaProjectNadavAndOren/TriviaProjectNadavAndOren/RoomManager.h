#pragma once
#include "Room.h"
#include <map>

class RoomManager
{
private:
	static std::map<unsigned int, Room> m_rooms;

public:
	void createRoom(RoomData rd, LoggedUser lu);
	void deleteRoom(int ID);
	unsigned int getRoomState(int ID);
	std::vector<RoomData> getRooms();
	Room& getRoom(int ID);
	static std::mutex m_roomsMutex;
	static int currId;
};