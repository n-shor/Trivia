#include "Game.h"
#include "RequestHandlerFactory.h"

std::map<std::string, clock_t> m_timeTracker;
std::mutex m_timeTrackerMutex;
std::mutex Game::m_playersMutex;
Question end = Question();

void Game::submitGameStatsToDB(GameData& gd, IDatabase* db)
{
	for (auto it = m_players.begin(); it != m_players.end(); ++it)
	{
		db->submitGameStatistics(it->second, it->first);
	}
}

Game::Game(Room& r, IDatabase* db, const unsigned int gameId)
{
	std::lock_guard<std::mutex> lock(m_playersMutex);
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
		auto& game = m_players[r.getAllUsers()[j]];
		game = GameData(m_questions[0], 0, 0, 0);
	}

	m_gameId = gameId;
}

Question Game::getQuestionForUser(std::string lu)
{
	std::lock_guard<std::mutex> lock(m_playersMutex);
	std::lock_guard<std::mutex> lock2(m_timeTrackerMutex);
	m_timeTracker[lu] = clock();
	auto& game = m_players[lu];
	return game.currentQuestion;
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
	std::lock_guard<std::mutex> lock(m_playersMutex);
	int ret = 0;
	std::cout << "submit answer" << lu << std::endl;
	if (m_players[lu].currentQuestion.getCorrectAnswerId() == id)
	{
		m_players[lu].correctAnswerCount++;
		if (getIndex(m_questions, m_players[lu].currentQuestion) + 1 < m_questions.size())
		{
			m_players[lu].currentQuestion = m_questions[getIndex(m_questions, m_players[lu].currentQuestion) + 1];
			ret = correctAnswer;
		}
		else
		{
			m_players[lu].currentQuestion = end;
			ret = playerFinished;
		}
		m_players[lu].AverageAnswerTime = (m_players[lu].AverageAnswerTime * m_players[lu].correctAnswerCount + double(clock() - m_timeTracker[lu])) / (m_players[lu].correctAnswerCount + 1);
		submitGameStatsToDB(m_players[lu], RequestHandlerFactory::getInstance().getStatisticsManager().getDB());
		return ret;
	}
	else {
		m_players[lu].wrongAnswerCount++;
		if (getIndex(m_questions, m_players[lu].currentQuestion) + 1 < m_questions.size())
		{
			m_players[lu].currentQuestion = m_questions[getIndex(m_questions, m_players[lu].currentQuestion) + 1];
			ret = incorrectAnswer;
		}
		else
		{
			m_players[lu].currentQuestion = end;
			ret = playerFinished;
		}
		submitGameStatsToDB(m_players[lu], RequestHandlerFactory::getInstance().getStatisticsManager().getDB());
		return ret;
	}
}

void Game::removePlayer(std::string lu)
{
	std::lock_guard<std::mutex> lock(m_playersMutex);
	auto ite = m_players.find(lu);
	if (ite != m_players.end()) {
		m_players.erase(ite);
	}

	auto ita = m_timeTracker.find(lu);
	if (ita != m_timeTracker.end()) {
		m_timeTracker.erase(ita);
	}
}

int Game::getGameId() const
{
	return m_gameId;
}

bool Game::operator==(const Game& other) const
{
	return m_gameId == other.m_gameId;
}

int Game::getCorrectAnswerId(std::string lu)
{
	return m_players[lu].currentQuestion.getCorrectAnswerId();
}

std::map<std::string, GameData>& Game::getPlayers()
{
	return m_players;
}

bool Game::hasGameEnded() const
{
	for (const auto& player : m_players)
	{
		if (player.second.currentQuestion != end)
		{
			return false;
		}
	}
	return true;
}
