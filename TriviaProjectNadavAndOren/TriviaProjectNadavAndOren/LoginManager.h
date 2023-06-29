#pragma once

#include "IDatabase.h"
#include "SqliteDatabase.hpp"
#include <string>
#include "LoggedUser.h"
#include <vector>
#include <mutex>

enum statuses { AlreadyLoggedIn = 36, Error = -1, FailedLogin = 37, UserAlreadyExists, LoggedIn, SignedUp, NoSuchLoggedUser, LoggedOut };

class LoginManager
{
protected:
    static std::unique_ptr<IDatabase> m_database;
    static std::mutex m_database_mutex;
public:
    LoginManager();
    int signup(std::string username, std::string password, std::string email);
    int login(std::string username, std::string password);
    int logout(std::string username);
    static std::vector<LoggedUser> m_loggedUsers;
    std::mutex m_loggedUsers_mutex;
};
