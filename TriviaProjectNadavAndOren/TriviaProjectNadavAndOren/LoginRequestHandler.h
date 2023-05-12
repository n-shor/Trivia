#pragma once

#include "IRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RequestHandlerFactory.h"

class LoginRequestHandler : public IRequestHandler
{
private:
    RequestHandlerFactory* m_handlerFactory;
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;

public:
    LoginRequestHandler();
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) override;

private:
    std::vector<unsigned char> login(const RequestInfo& requestInfo);
    std::vector<unsigned char> signup(const RequestInfo& requestInfo);
};
