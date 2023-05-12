#pragma once

#include "LoginManager.h"
class LoginRequestHandler; // Forward declarations to avoid circular dependency problems
class MenuRequestHandler;

class RequestHandlerFactory
{
protected:
    LoginManager m_loginManager;
public:
    RequestHandlerFactory();
    LoginRequestHandler* createLoginRequestHandler();
    MenuRequestHandler* createMenuRequestHandler();
    LoginManager& getLoginManager();
};
