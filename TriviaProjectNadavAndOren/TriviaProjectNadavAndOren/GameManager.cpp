#include "GameManager.h"
unsigned int GameManager::gameId = 0;
std::mutex GameManager::gameLock;

GameManager::GameManager(IDatabase* db) : m_database(db)
{
}

Game& GameManager::createGame(Room& r)
{
    Game g(r, m_database, gameId);
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

Game& GameManager::findUserGame(std::string lu)
{
    std::lock_guard<std::mutex> lock(gameLock);
    auto it = m_games[0].getPlayers().find(lu);
    for (int a = 0; a < m_games.size(); a++)
    {
        it = m_games[a].getPlayers().find(lu);
        //memory error nadav
        if (it != m_games[a].getPlayers().end()) {
            return m_games[a];
        }
    }
}
