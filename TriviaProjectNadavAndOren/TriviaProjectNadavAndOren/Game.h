#pragma once
#include "Room.h"
#include "IRequestHandler.h"
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"

enum submitAnswerStatus { incorrectAnswer, correctAnswer};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;
	unsigned int gameId;
	void submitGameStatsToDB(GameData, IDatabase*);
public:
	Game(Room, IDatabase*, unsigned int);
	Question getQuestionForUser(std::string);
	int submitAnswer(std::string, unsigned int id);
	void removePlayer(std::string);
	int getGameId();
	int getCorrectAnswerId(std::string);
	std::map<std::string, GameData> getPlayers();
};