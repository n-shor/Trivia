#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "IDatabase.h"
#include <string>
#include <vector>

class User {
public:
    std::string username;
    std::string password;
    std::string email;

    User(std::string username, std::string password, std::string email) : username(username), password(password), email(email) {}
};

class DatabaseLoginManager {
private:
    std::vector<User> connected_users;
    IDatabase* database;

public:
    DatabaseLoginManager(IDatabase* db) : database(db) {}

    void login(std::string username, std::string password) {
        if (database->doesUserExist(username) && database->doesPasswordMatch(username, password)) {
            User user(username, password, "");
            connected_users.push_back(user);
        }
        // Add error handling or other state changes as needed
    }

    void signup(std::string username, std::string password, std::string email) {
        if (!database->doesUserExist(username)) {
            database->addNewUser(username, password, email);
        }
        // Add error handling or other state changes as needed
    }

    void disconnect(std::string username) {
        for (auto it = connected_users.begin(); it != connected_users.end(); ++it) {
            if (it->username == username) {
                connected_users.erase(it);
                break;
            }
        }
    }
};

class LoginRequestHandler : public IRequestHandler
{
private:
    DatabaseLoginManager db_login_manager;
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;

public:
    LoginRequestHandler(IDatabase* db) : db_login_manager(db) {}

    bool isRequestRelevant(const RequestInfo& requestInfo) override {
        return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
    }

    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override {
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
    std::vector<unsigned char> login(const RequestInfo& requestInfo) {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
        db_login_manager.login(loginRequest.username, loginRequest.password);

        LoginResponse l;
        l.status = 1;

        return JsonResponsePacketSerializer::serializeLoginResponse(l);
    }

    std::vector<unsigned char> signup(const RequestInfo& requestInfo) {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
        db_login_manager.signup(signupRequest.username, signupRequest.password, signupRequest.email);

        SignupResponse l;
        l.status = 1;

        return JsonResponsePacketSerializer::serializeSignUpResponse(l);
    }
};