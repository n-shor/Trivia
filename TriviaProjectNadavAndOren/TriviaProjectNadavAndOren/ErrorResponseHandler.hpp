#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.hpp"
#include "JsonResponsePacketSerializer.hpp"
#include "user.h"

class ErrorResponseHandler : public IRequestHandler
{
    bool isRequestRelevant(const RequestInfo& requestInfo) override {
        return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
    }

    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override {
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
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;
};
