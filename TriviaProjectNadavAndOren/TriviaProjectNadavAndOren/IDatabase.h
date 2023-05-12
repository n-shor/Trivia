#pragma once
#include <string>

class IDatabase 
{
public:
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool doesUserExist(std::string username) = 0;
    virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
    virtual bool addNewUser(std::string username, std::string password, std::string userDetails) = 0;
    virtual ~IDatabase() {}
};
