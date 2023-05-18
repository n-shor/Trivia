#pragma once
#include <vector>
#include <string>
#include "SqliteDatabase.hpp"

class StatisticsManager
{
private:
    IDatabase* m_database;

public:
    StatisticsManager(IDatabase* db) : m_database(db)
    {
    }

    std::vector<std::string> getHighScore()
    {
        return m_database->getHighScores();
    }

    std::vector<std::string> getUserStatistics(std::string username)
    {
        std::vector<std::string> result;

        result.push_back("Average Answer Time: " + std::to_string(m_database->getPlayerAverageAnswerTime(username)));
        result.push_back("Number of Correct Answers: " + std::to_string(m_database->getNumOfCorrectAnswers(username)));
        result.push_back("Number of Total Answers: " + std::to_string(m_database->getNumOfTotalAnswers(username)));
        result.push_back("Number of Played Games: " + std::to_string(m_database->getNumOfPlayerGames(username)));
        result.push_back("Player Score: " + std::to_string(m_database->getPlayerScore(username)));

        return result;
    }
};
