#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
    m_handlerFactory = new RequestHandlerFactory(new SqliteDatabase());
}


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.messageCode == LOGIN_CODE)
    {
        return login(requestInfo);
    }
    else if (requestInfo.messageCode == SIGNUP_CODE)
    {
        return signup(requestInfo);
    }

    RequestResult r;
    ErrorResponse e;
    e.message = "irrelevant message";
    r.response = JsonResponsePacketSerializer::serializeResponse(e);
    r.newHandler = NULL;
    return r;
}

RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
    RequestResult r;
    LoginResponse l;
    l.status = m_handlerFactory->getLoginManager().login(loginRequest.username, loginRequest.password);
    r.response = JsonResponsePacketSerializer::serializeResponse(l);
    r.newHandler = m_handlerFactory->createMenuRequestHandler();
    return r;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
    SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
    RequestResult r;
    SignupResponse l;
    l.status = m_handlerFactory->getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);
    r.response = JsonResponsePacketSerializer::serializeResponse(l);
    r.newHandler = m_handlerFactory->createLoginRequestHandler();
    return r;
}
