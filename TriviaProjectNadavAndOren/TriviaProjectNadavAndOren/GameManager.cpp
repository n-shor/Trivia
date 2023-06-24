#include "GameManager.h"
unsigned int gameId = 0;
Game GameManager::createGame(Room r)
{
    Game g = Game(r, m_database, gameId);
    m_games.push_back(g);
    gameId++;
    return g;
}

void GameManager::deleteGame(int gameId)
{
    std::lock_guard<std::mutex> lock(gameLock);
    for (auto it = m_games.begin(); it != m_games.end();)
    {
        if (it->getGameId() == gameId)
        {
            m_games.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
}
