#pragma once
#include <string>
#include <vector>
#include "Question.h"
#include "IRequestHandler.h"

class IDatabase 
{
public:
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool doesUserExist(std::string username) = 0;
    virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
    virtual bool addNewUser(std::string username, std::string password, std::string userDetails) = 0;
    virtual bool addQuestion(int id, std::string question, std::string optionA, std::string optionB,
    std::string optionC, std::string optionD, char correctAnswer) = 0;
    virtual Question getQuestion(int id) = 0;
    virtual char getCorrectAnswer(int id) = 0;
    virtual float getPlayerAverageAnswerTime(std::string username) = 0;
    virtual int getNumOfCorrectAnswers(std::string username) = 0;
    virtual int getNumOfTotalAnswers(std::string username) = 0;
    virtual int getNumOfPlayerGames(std::string username) = 0;
    virtual int getPlayerScore(std::string username) = 0;
    virtual std::vector<std::string> getHighScores() = 0;
    virtual bool submitGameStatistics(GameData gd, std::string username) = 0;
    virtual int getQuestionCount() = 0;
    virtual ~IDatabase() {}
};
