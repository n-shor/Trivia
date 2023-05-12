#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler()
{
    m_handlerFactory = new RequestHandlerFactory();
}


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.messageCode == LOGIN_CODE || requestInfo.messageCode == SIGNUP_CODE;
}

std::vector<unsigned char> LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
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

std::vector<unsigned char> LoginRequestHandler::login(const RequestInfo& requestInfo)
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo);
    LoginResponse l;
    l.status = m_handlerFactory->getLoginManager().login(loginRequest.username, loginRequest.password);
    return JsonResponsePacketSerializer::serializeLoginResponse(l);
}

std::vector<unsigned char> LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
    SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo);
    SignupResponse l;
    l.status = m_handlerFactory->getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);
    return JsonResponsePacketSerializer::serializeSignUpResponse(l);
}
