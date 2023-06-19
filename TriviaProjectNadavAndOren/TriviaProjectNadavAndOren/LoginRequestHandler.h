#pragma once

#include "IRequestHandler.h"
#include "JsonResponsePacketSerializer.hpp"
#include "JsonRequestPacketDeserializer.hpp"
#include "RequestHandlerFactory.h"

class LoginRequestHandler : public IRequestHandler
{
private:
    constexpr static int LOGIN_CODE = 1;
    constexpr static int SIGNUP_CODE = 2;
    constexpr static int END_CODE = 3;

public:
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult login(const RequestInfo& requestInfo);
    RequestResult signup(const RequestInfo& requestInfo);
};
