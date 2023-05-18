#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"

class Room
{
private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
public:
    Room(RoomData);
    void addUser(LoggedUser);
    void removeUser(LoggedUser);
    std::vector<std::string> getAllUsers();
    RoomData getRoomData();
};