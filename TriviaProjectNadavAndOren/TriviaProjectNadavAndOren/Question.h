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
    Question(std::string m_question, std::vector<std::string> m_possibleAnswers, int correctAnswerId);
    Question();
    const std::string getQuestion() const;
    const std::vector<std::string> getPossibleAnswers() const;
    const int getCorrectAnswerId() const;
    bool operator==(const Question& c); 
};