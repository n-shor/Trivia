#pragma once
#include "Room.h"
#include "IRequestHandler.h"
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"
#include <mutex>
#include <ctime>

const std::string notEnoughQuestionsMessage = "the server doesnt have enough questions for the requested game";

enum submitAnswerStatus { incorrectAnswer = 11, correctAnswer};

class Game
{
private:
	Room m_room;
	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;
	static std::mutex m_playersMutex;
	unsigned int m_gameId;
	void submitGameStatsToDB(GameData&, IDatabase*);

public:
	Game(Room&, IDatabase*, const unsigned int);
	Question getQuestionForUser(const std::string);
	int submitAnswer(const std::string, const unsigned int id);
	void removePlayer(const std::string);
	int getGameId() const;
	bool operator==(const Game& other) const;
	const int getCorrectAnswerId(const std::string);
	const std::map<std::string, GameData>& getPlayers() const;
	bool hasGameEnded() const;
	int getRoomId() const;
	static int constexpr playerFinished = 250;
};