#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "JsonResponsePacketSerializer.hpp"
#include "user.h"


class RegisterRequestHandler : public IRequestHandler
{
    bool isRequestRelevant(const RequestInfo& requestInfo) override {
        return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
    }

    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
        std::cout << "username: " << signupRequest.username << ", email: " << signupRequest.email << ", password: " << signupRequest.password << std::endl;
        // Process the signup request and generate a response
        // TODO: continue the code with logic here

        user* new_user = new user(signupRequest.username, signupRequest.email, signupRequest.password);
        
        SignupResponse l;
        l.status = 69;

        return JsonResponsePacketSerializer::serializeSignUpResponse(l);
    }

private:
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;
};
