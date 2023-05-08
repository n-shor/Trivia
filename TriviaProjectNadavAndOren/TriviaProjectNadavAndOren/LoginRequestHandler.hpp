#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "user.h"

class LoginRequestHandler : public IRequestHandler
{
    bool isRequestRelevant(const RequestInfo& requestInfo) override {
        return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
    }

    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
        std::cout << "username: " << loginRequest.username  << ", password: " << loginRequest.password << std::endl;
        // Process the login request and generate a response
        // TODO: continue the code with logic here
        
        // If the request is not relevant, set the next handler and return an empty vector
        //nextHandler = std::make_shared<NextRequestHandler>();
        LoginResponse l;
        l.status = 69;

        return JsonResponsePacketSerializer::serializeLoginResponse(l);
    }

private:
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;
};
