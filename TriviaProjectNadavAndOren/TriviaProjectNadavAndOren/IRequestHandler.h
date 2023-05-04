#pragma once
#include <memory>
#include <string>
#include <chrono>

struct LoginRequest
{
    std::string username;
    std::string password;
};

struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
};

struct LoginResponse
{
    int status;
};

struct SignupResponse
{
    int status;
};

struct ErrorResponse
{
    std::string message;
};

struct RequestInfo
{
    int messageCode;
    std::chrono::time_point<std::chrono::system_clock> receivedTime;
    std::vector<unsigned char> messageContent;
};

class IRequestHandler
{
public:
    virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
    virtual std::vector<unsigned char> handleRequest(const RequestInfo& requestInfo) = 0;
};