#pragma once
#include "Game.h"
#include "Room.h"

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
	static std::mutex gameLock;
	static unsigned int gameId;
public:
	std::vector<Game>& getGames();
	GameManager(IDatabase*);
	int createGame(Room&);
	void deleteGame(const int gameId);
	Game& findUserGame(const std::string);
};