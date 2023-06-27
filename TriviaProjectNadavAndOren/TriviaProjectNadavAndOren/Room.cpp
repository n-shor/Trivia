#include "Room.h"

Room::Room()
{

}

Room::Room(RoomData& d) : m_metadata(d)
{
}

void Room::addUser(LoggedUser u)
{
	m_users.push_back(u);
    this->m_metadata.currentPlayers++;
}

void Room::removeUser(LoggedUser u)
{
    auto temp = m_users.begin();
    this->m_metadata.currentPlayers--;
    for (auto it = m_users.begin(); it != m_users.end(); it++)
    {
        if (it->getUsername() == u.getUsername())
        {
            m_users.erase(it);
            return;
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

RoomData& Room::getRoomData()
{
    return m_metadata;
}

void Room::activateRoom()
{
    m_metadata.isActive = 1;
}
