#include "Question.h"

Question::Question(std::string m_question, std::vector<std::string> m_possibleAnswers) : m_question(m_question), m_possibleAnswers(m_possibleAnswers)
{
}

std::string Question::getQuestion()
{
	return m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
	return m_possibleAnswers;
}

int Question::getCorrectAnswerId()
{
	return correctAnswerId;
}
