#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == LOGIN_CODE
        || requestInfo.messageCode == SIGNUP_CODE
        || requestInfo.messageCode == END_CODE;
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
    else if (requestInfo.messageCode == END_CODE)
    {
        RequestResult r;
        ErrorResponse response;
        response.message = "End of messages from client";
        r.response = JsonResponsePacketSerializer::serializeResponse(response);
        r.newHandler = nullptr;
        return r;
    }

    // Return an irrelevant response for unrecognized message codes
    RequestResult r;
    ErrorResponse e;
    e.message = "Irrelevant message";
    r.response = JsonResponsePacketSerializer::serializeResponse(e);
    r.newHandler = RequestHandlerFactory::getInstance().createLoginRequestHandler();
    return r;
}



RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
    RequestResult r;
    LoginResponse l;
    l.status = RequestHandlerFactory::getInstance().getLoginManager().login(loginRequest.username, loginRequest.password);
    r.response = JsonResponsePacketSerializer::serializeResponse(l);
    if (l.status == LoggedIn)
    {
        r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(loginRequest.username);
        r.username = loginRequest.username;
    }
    else
    {
        r.newHandler = RequestHandlerFactory::getInstance().createLoginRequestHandler();
    }
    return r;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
    SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
    RequestResult r;
    SignupResponse l;
    l.status = RequestHandlerFactory::getInstance().getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);
    r.response = JsonResponsePacketSerializer::serializeResponse(l);
    if (l.status == SignedUp)
    {
        r.newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(signupRequest.username);
        r.username = signupRequest.username;
    }
    else
    {
        r.newHandler = RequestHandlerFactory::getInstance().createLoginRequestHandler();
    }
    return r;
}
