#pragma once
#include "Game.h"
#include "Room.h"

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	std::mutex gameLock;
	Game createGame(Room);
	void deleteGame(int gameId);
};