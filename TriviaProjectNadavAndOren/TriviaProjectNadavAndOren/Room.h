#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"

enum roomStates {isActive, isntActive};

class Room
{
private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
public:
    Room();
    Room(RoomData);
    void addUser(LoggedUser);
    void removeUser(LoggedUser);
    std::vector<std::string> getAllUsers();
    RoomData getRoomData();
};