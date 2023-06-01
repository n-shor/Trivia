#pragma once

#include <string>

class Question {
public:
    int id;
    std::string question;
    std::string optionA;
    std::string optionB;
    std::string optionC;
    std::string optionD;
    char correctAnswer;

    Question() {}

    Question(int id, std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD, char correctAnswer) :
        id(id), question(question), optionA(optionA), optionB(optionB), optionC(optionC), optionD(optionD), correctAnswer(correctAnswer) {}
};
