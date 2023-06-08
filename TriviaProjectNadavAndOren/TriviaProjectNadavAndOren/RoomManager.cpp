#include "RoomManager.h"


void RoomManager::createRoom(LoggedUser lu, RoomData rd)
{
	m_rooms[rd.id] = *(new Room(rd));
}

void RoomManager::deleteRoom(int ID)
{
    for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
    {
        if (it->first == ID)
        {
            m_rooms.erase(it);
        }
    }
}

unsigned int RoomManager::getRoomState(int ID)
{
    return m_rooms[ID].getRoomData().isActive;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> ret;
    if (!m_rooms.empty()) //for some reason the loop runs even when it's empty
    {
        for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it) {
            ret.push_back(it->second.getRoomData());
        }
    }
    return ret;
}

Room& RoomManager::getRoom(int ID)
{
    return m_rooms[ID];
}
