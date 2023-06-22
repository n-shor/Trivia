#pragma once
#include "IRequestHandler.h"
#include <map>
#include "LoggedUser.h"

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_player;
	unsigned int gameId;
	void sumitGameStatsToDB(GameData);
public:
	Question getQuestionForUser(LoggedUser);
	int submitAnswer();
	int removePlayer();
};