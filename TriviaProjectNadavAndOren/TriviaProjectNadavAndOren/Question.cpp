#include "Question.h"

Question::Question(std::string m_question, std::vector<std::string> m_possibleAnswers, int correctAnswerId) : m_question(m_question), m_possibleAnswers(m_possibleAnswers), correctAnswerId(correctAnswerId)
{
}

Question::Question()
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

bool Question::operator==(const Question& c)
{
	if (correctAnswerId == c.correctAnswerId && m_question == c.m_question && m_possibleAnswers == c.m_possibleAnswers)
		return true;
	return false;
}
