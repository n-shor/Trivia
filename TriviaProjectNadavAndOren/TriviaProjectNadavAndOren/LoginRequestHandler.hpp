#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "user.h"

class LoginRequestHandler : public IRequestHandler
{
public:
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
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
        std::cout << "username: " << signupRequest.username << ", email: " << signupRequest.email << ", password: " << signupRequest.password << std::endl;
        // Process the signup request and generate a response
        // TODO: continue the code with logic here

        // If the request is not relevant, set the next handler and return an empty vector
        //nextHandler = std::make_shared<NextRequestHandler>();
        ErrorResponse e;
        e.message = "irrelevant message";
        std::cout << "cancer";
        return JsonResponsePacketSerializer::serializeErrorResponse(e);
    }

private:
    std::vector<unsigned char> login(const RequestInfo& requestInfo) {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
        std::cout << "username: " << loginRequest.username << ", password: " << loginRequest.password << std::endl;
        // Process the login request and generate a response
        // TODO: continue the code with logic here

        // If the request is not relevant, set the next handler and return an empty vector
        //nextHandler = std::make_shared<NextRequestHandler>();
        LoginResponse l;
        l.status = 1;

        return JsonResponsePacketSerializer::serializeLoginResponse(l);
    }
    std::vector<unsigned char> signup(const RequestInfo& requestInfo) {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
        std::cout << "username: " << signupRequest.username << ", email: " << signupRequest.email << ", password: " << signupRequest.password << std::endl;
        // Process the signup request and generate a response
        // TODO: continue the code with logic here

        user* new_user = new user(signupRequest.username, signupRequest.email, signupRequest.password);

        SignupResponse l;
        l.status = 1;

        return JsonResponsePacketSerializer::serializeSignUpResponse(l);
    }
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;
};