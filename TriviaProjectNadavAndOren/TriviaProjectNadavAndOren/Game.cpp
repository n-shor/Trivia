#include "Game.h"
#include "RequestHandlerFactory.h"

void Game::submitGameStatsToDB(GameData gd, IDatabase* db)
{
	for (auto it = m_players.begin(); it != m_players.end(); ++it)
	{
		db->submitGameStatistics(it->second, it->first);
	}
}

Game::Game(Room& r, IDatabase* db, unsigned int gameId)
{
	if (r.getRoomData().numOfQuestionsInGame > db->getQuestionCount())
	{
		throw(69);
	}

	for (int i = 0; i < r.getRoomData().numOfQuestionsInGame; i++)
	{
		m_questions.push_back(db->getQuestion(i+1));
	}

	for (int j = 0; j < r.getAllUsers().size(); j++)
	{
		auto& a = m_players[r.getAllUsers()[j]];
		a = GameData(m_questions[0], 0, 0, 0);
	}

	this->gameId = gameId;
}

Question Game::getQuestionForUser(std::string lu)
{
	m_timeTracker[lu] = clock();
	return m_players[lu].currentQuestion;
}

int getIndex(std::vector<Question> v, Question K)
{
    auto it = find(v.begin(), v.end(), K);

    if (it != v.end())
    {

        return it - v.begin();
    }
    else {
		return -1;
    }
}

int Game::submitAnswer(std::string lu, unsigned int id)
{
	if (m_players[lu].currentQuestion.getCorrectAnswerId() == id)
	{
		m_players[lu].correctAnswerCount++;
		m_players[lu].currentQuestion = m_questions[getIndex(m_questions, m_players[lu].currentQuestion) + 1];
		m_players[lu].AverageAnswerTime = (m_players[lu].AverageAnswerTime * m_players[lu].correctAnswerCount + double(clock() - m_timeTracker[lu])) / (m_players[lu].correctAnswerCount + 1);
		submitGameStatsToDB(m_players[lu], RequestHandlerFactory::getInstance().getStatisticsManager().getDB());
		return correctAnswer;
	}
	else {
		m_players[lu].wrongAnswerCount++;
		m_players[lu].currentQuestion = m_questions[getIndex(m_questions, m_players[lu].currentQuestion) + 1];
		submitGameStatsToDB(m_players[lu], RequestHandlerFactory::getInstance().getStatisticsManager().getDB());
		return incorrectAnswer;
	}
}

void Game::removePlayer(std::string lu)
{
	auto ite = m_players.find(lu);
	if (ite != m_players.end()) {
		m_players.erase(ite);
	}

	auto ita = m_timeTracker.find(lu);
	if (ita != m_timeTracker.end()) {
		m_timeTracker.erase(ita);
	}
}

int Game::getGameId()
{
	return gameId;
}

int Game::getCorrectAnswerId(std::string lu)
{
	return m_players[lu].currentQuestion.getCorrectAnswerId();
}

std::map<std::string, GameData> Game::getPlayers()
{
	return m_players;
}
