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
	GameManager(IDatabase*);
	Game& createGame(Room&);
	void deleteGame(int gameId);
	Game& findUserGame(std::string);
};