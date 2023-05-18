#include "Room.h"

Room::Room(RoomData d)
{
    m_metadata = d;
}

void Room::addUser(LoggedUser u)
{
	m_users.push_back(u);
}

void Room::removeUser(LoggedUser u)
{
    for (auto it = m_users.begin(); it != m_users.end(); it++)
    {
        if (it->getUsername() == u.getUsername())
        {
            m_users.erase(it);
        }
    }
}

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> ret;
    for (int it = 0; it < m_users.size(); it++)
    {
        ret.push_back(m_users[it].getUsername());
    }
    return ret;
}

RoomData Room::getRoomData()
{
    return m_metadata;
}
