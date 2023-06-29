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
    virtual bool doesUserExist(const std::string username) const = 0;
    virtual bool doesPasswordMatch(const std::string username, const std::string password) const = 0;
    virtual bool addNewUser(const std::string username, const std::string password, const std::string userDetails) = 0;
    virtual bool addQuestion(const int id, const std::string question, const std::string optionA, const std::string optionB,
        const std::string optionC, const std::string optionD, const char correctAnswer) = 0;
    virtual Question getQuestion(const int id) const = 0;
    virtual char getCorrectAnswer(const int id) const = 0;
    virtual float getPlayerAverageAnswerTime(const std::string username) const = 0;
    virtual int getNumOfCorrectAnswers(const std::string username) const = 0;
    virtual int getNumOfTotalAnswers(const std::string username) const = 0;
    virtual int getNumOfPlayerGames(const std::string username) const = 0;
    virtual int getPlayerScore(const std::string username) const = 0;
    virtual std::vector<std::string> getHighScores() const = 0;
    virtual bool submitGameStatistics(const GameData gd, const std::string username) = 0;
    virtual int getQuestionCount() const = 0;
    virtual ~IDatabase() {}
};
