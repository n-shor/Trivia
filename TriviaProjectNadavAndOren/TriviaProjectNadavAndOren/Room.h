#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"

enum roomStates {isActive = 1, isntActive = 0};

class Room
{
private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
public:
    Room();
    Room(RoomData&);
    void addUser(const LoggedUser);
    void removeUser(const LoggedUser);
    const std::vector<std::string> getAllUsers();
    const RoomData& getRoomData() const;
    void activateRoom();
    void deactivateRoom();
};