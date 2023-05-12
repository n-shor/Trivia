#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "IDatabase.h"
#include <string>
#include <vector>
#include "SqliteDatabase.hpp"

enum statuses { Error = -1, FailedLogin = 1234, UserAlreadyExists = 42, LoggedIn = 69, SignedUp = 420, NoSuchLoggedUser = 567, LoggedOut = 890};


class LoggedUser
{
private:
    std::string m_username;
public:
    LoggedUser(std::string username)
    {
        m_username = username;
    }
    std::string getUsername()
    {
        return m_username;
    }
};


class LoginManager 
{
protected:
    IDatabase* m_database;
    std::vector<LoggedUser> m_LoggedUsers;
public:
    LoginManager() 
    {
        m_database = new SqliteDatabase();
        m_database->open();
    }

    int signup(std::string username, std::string password, std::string email)
    {
        if (!m_database->doesUserExist(username)) {
            m_database->addNewUser(username, password, email);
            return SignedUp;
        }
        return UserAlreadyExists;
        // Add error handling or other state changes as needed
    }
    int login(std::string username, std::string password)
    {
        if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password)) 
        {
            m_LoggedUsers.push_back(*(new LoggedUser(username)));
            return LoggedIn;
        }
        return FailedLogin;
    }
    int logout(std::string username)
    {
        for (auto it = m_LoggedUsers.begin(); it != m_LoggedUsers.end(); ++it) 
        {
            if (it->getUsername() == username) 
            {
                m_LoggedUsers.erase(it);
                return LoggedOut;
            }
        }
        return NoSuchLoggedUser;
    }

    ~LoginManager()
    {
        m_database->close();
        delete m_database;
    }
};

class MenuRequestHandler{}; //is only empty for now, until we have a menu

class RequestHandlerFactory
{
protected:
    LoginManager m_loginManager;
public:
    RequestHandlerFactory()
    {
        m_loginManager = *(new LoginManager());
    }
    LoginRequestHandler* createLoginRequestHandler()
    {
        return new LoginRequestHandler();
    }
    MenuRequestHandler* createMenuRequestHandler()
    {
        return NULL;
    }
    LoginManager& getLoginManager()
    {
        return m_loginManager;
    }
};

class LoginRequestHandler : public IRequestHandler
{
private:
    RequestHandlerFactory m_handlerFactory;
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;

public:
    LoginRequestHandler() 
    {
        m_handlerFactory = *(new RequestHandlerFactory());
    }

    bool isRequestRelevant(const RequestInfo& requestInfo) override
    {
        return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
    }

    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override 
    {
        if (requestInfo.messageCode == LOGIN_CODE)
        {
            return login(requestInfo);
        }
        else if (requestInfo.messageCode == SIGNUP_CODE)
        {
            return signup(requestInfo);
        }

        ErrorResponse e;
        e.message = "irrelevant message";
        return JsonResponsePacketSerializer::serializeErrorResponse(e);
    }

private:
    std::vector<unsigned char> login(const RequestInfo& requestInfo) 
    {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);

        LoginResponse l;

        l.status = m_handlerFactory.getLoginManager().login(loginRequest.username, loginRequest.password);

        return JsonResponsePacketSerializer::serializeLoginResponse(l);
    }

    std::vector<unsigned char> signup(const RequestInfo& requestInfo) 
    {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);

        SignupResponse l;

       l.status = m_handlerFactory.getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);


        return JsonResponsePacketSerializer::serializeSignUpResponse(l);
    }
};