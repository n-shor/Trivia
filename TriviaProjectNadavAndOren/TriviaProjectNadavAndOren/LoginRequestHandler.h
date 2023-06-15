#pragma once

#include "IRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RequestHandlerFactory.h"

class LoginRequestHandler : public IRequestHandler
{
private:
    RequestHandlerFactory& m_handlerFactory;
    static const int LOGIN_CODE = 1;
    static const int SIGNUP_CODE = 2;
    static const int END_CODE = 3;

public:
    LoginRequestHandler(RequestHandlerFactory& rhf);
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult login(const RequestInfo& requestInfo);
    RequestResult signup(const RequestInfo& requestInfo);
};
