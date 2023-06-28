#include "Question.h"

Question::Question(std::string m_question, std::vector<std::string> m_possibleAnswers, int correctAnswerId) : m_question(m_question), m_possibleAnswers(m_possibleAnswers), correctAnswerId(correctAnswerId)
{
}

Question::Question()
{
}

const std::string Question::getQuestion() const
{
	return m_question;
}

const std::vector<std::string> Question::getPossibleAnswers() const
{
	return m_possibleAnswers;
}

const int Question::getCorrectAnswerId() const
{
	return correctAnswerId;
}

bool Question::operator==(const Question& c)
{
	if (correctAnswerId == c.correctAnswerId && m_question == c.m_question && m_possibleAnswers == c.m_possibleAnswers)
		return true;
	return false;
}
