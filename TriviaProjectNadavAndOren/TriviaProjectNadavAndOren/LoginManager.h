#pragma once

#include "IDatabase.h"
#include "SqliteDatabase.hpp"
#include <string>
#include <vector>

class LoggedUser;

enum statuses { Error = -1, FailedLogin = 1234, UserAlreadyExists = 42, LoggedIn = 69, SignedUp = 420, NoSuchLoggedUser = 567, LoggedOut = 890 };

class LoginManager
{
protected:
    IDatabase* m_database;
    std::vector<LoggedUser> m_LoggedUsers;
public:
    LoginManager();
    int signup(std::string username, std::string password, std::string email);
    int login(std::string username, std::string password);
    int logout(std::string username);
    ~LoginManager();
};
