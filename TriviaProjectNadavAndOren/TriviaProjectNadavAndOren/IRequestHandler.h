#pragma once
#include <memory>
#include <string>
#include <chrono>
#include <vector>
#include "IRequestHandler.h"

class IRequestHandler;

struct CloseRoomResponse
{
    unsigned int status;
};

struct StartGameResponse
{
    unsigned int status;
};

struct GetRoomStateResponse
{
    unsigned int status;
    bool hasGameBegun;
    std::vector<std::string> players;
    unsigned int questionCount;
    unsigned int answerTimeout;
};

struct LeaveRoomResponse
{
    unsigned int status;
};

struct RoomData
{
    unsigned int id;
    std::string name;
    std::string adminName;
    unsigned int currentPlayers;
    unsigned int maxPlayers;
    unsigned int numOfQuestionsInGame;
    unsigned int timePerQuestion;
    unsigned int isActive;
};

struct CreateRoomRequest
{
    std::string roomName;
    unsigned int maxUsers;
    unsigned int questionCount;
    unsigned int answerTimeout;
};

struct GetPlayersInRoomRequest
{
    unsigned int roomId;
};

struct JoinRoomRequest
{
    unsigned int roomId;
};

struct RequestResult
{
    std::vector<unsigned char> response;
    std::unique_ptr<IRequestHandler> newHandler;
};

struct GetPlayesInRoomResponse
{
    std::vector<std::string> players;
};

struct getHighScoreResponse
{
    unsigned int status;
    std::vector<std::string> statistics;
};

struct getPersonalStatsResponse
{
    unsigned int status;
    std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
    unsigned int status;
};

struct CreateRoomResponse
{
    unsigned int status;
    unsigned int roomId;
    std::string adminName;
};

struct GetRoomsResponse
{
    unsigned int status;
    std::vector<RoomData> rooms;
};

struct LogoutResponse
{
    unsigned int status;
};

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
    unsigned int status;
};

struct SignupResponse
{
    unsigned int status;
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
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};