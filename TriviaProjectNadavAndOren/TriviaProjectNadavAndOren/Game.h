#pragma once
#include "Room.h"
#include "IRequestHandler.h"
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"

enum submitAnswerStatus { incorrectAnswer = 11, correctAnswer};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;
	unsigned int m_gameId;
	void submitGameStatsToDB(GameData, IDatabase*);
public:
	Game(Room&, IDatabase*, const unsigned int);
	Question getQuestionForUser(const std::string);
	int submitAnswer(const std::string, const unsigned int id);
	void removePlayer(const std::string);
	int getGameId() const;
	bool operator==(const Game& other) const;
	int getCorrectAnswerId(const std::string);
	std::map<std::string, GameData>& getPlayers();
	bool hasGameEnded() const;
};