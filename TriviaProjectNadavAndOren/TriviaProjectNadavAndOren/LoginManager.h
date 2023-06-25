#pragma once

#include "IDatabase.h"
#include "SqliteDatabase.hpp"
#include <string>
#include "LoggedUser.h"
#include <vector>
#include <mutex>

enum statuses { AlreadyLoggedIn = 17, Error = -1, FailedLogin = 21, UserAlreadyExists = 22, LoggedIn = 11, SignedUp = 12, NoSuchLoggedUser = 23, LoggedOut = 13 };

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
