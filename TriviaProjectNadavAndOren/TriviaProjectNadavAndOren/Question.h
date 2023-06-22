#pragma once
#include <memory>
#include <string>
#include <chrono>
#include <vector>

class Question
{
private:
    int correctAnswerId;
    std::string m_question;
    std::vector<std::string> m_possibleAnswers;
public:
    Question(std::string m_question, std::vector<std::string> m_possibleAnswers);
    std::string getQuestion();
    std::vector<std::string> getPossibleAnswers();
    int getCorrectAnswerId();
};