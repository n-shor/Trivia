#pragma once
#include <memory>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include "Question.h"

class IRequestHandler;

struct leaderBoardResponse
{
    std::map<std::string, unsigned int> players;
};

struct AddQuestionRequest {
    std::string question;
    std::string optionA;
    std::string optionB;
    std::string optionC;
    std::string optionD;
    char correctAnswer;
};

struct AddQuestionResponse {
    unsigned int status;
};

struct SubmitAnswerRequest
{
    unsigned int answerId;
};

struct GameData
{
    Question currentQuestion;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    double AverageAnswerTime;
    GameData(Question currentQuestion, unsigned int correctAnswerCount, unsigned int wrongAnswerCount, double AverageAnswerTime) : currentQuestion(currentQuestion), correctAnswerCount(correctAnswerCount), wrongAnswerCount(wrongAnswerCount), AverageAnswerTime(AverageAnswerTime){}
    GameData() : currentQuestion() {}
};

struct PlayerResults
{
    std::string username;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    double averageAnswerTime;
    PlayerResults(std::string username, unsigned int correctAnswerCount, unsigned int wrongAnswerCount, double averageAnswerTime) : username(username), correctAnswerCount(correctAnswerCount), wrongAnswerCount(wrongAnswerCount), averageAnswerTime(averageAnswerTime){}
};

struct LeaveGameResponse
{
    unsigned int status;
};

struct GetQuestionResponse
{
    unsigned int status;
    std::string question;
    std::map<unsigned int, std::string> answers;
};

struct SubmitAnswerResponse
{
    unsigned int status;
    unsigned int correctAnswerId;
};

struct GetGameResultsResponse
{
    unsigned int status;
    std::vector<PlayerResults> results;
};

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
    RoomData() {}
    RoomData(unsigned int id, std::string name, std::string adminName, unsigned int currentPlayers, unsigned int maxPlayers, unsigned int numOfQuestionsInGame, unsigned int timePerQuestion, unsigned int isActive) : id(id), name(name), adminName(adminName), currentPlayers(currentPlayers), maxPlayers(maxPlayers), numOfQuestionsInGame(numOfQuestionsInGame), timePerQuestion(timePerQuestion), isActive(isActive) {}
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
    std::string username;
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