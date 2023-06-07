#include "LoginManager.h"
#include "LoggedUser.h"
#include <mutex>

std::unique_ptr<IDatabase> LoginManager::m_database = nullptr;
std::mutex LoginManager::m_database_mutex;

LoginManager::LoginManager()
{
    std::lock_guard<std::mutex> lock(m_database_mutex);
    if (m_database == nullptr) {
        m_database = std::make_unique<SqliteDatabase>();
        m_database->open();
    }
}

int LoginManager::signup(std::string username, std::string password, std::string email)
{
    std::lock_guard<std::mutex> lock(m_database_mutex);
    if (!m_database->doesUserExist(username)) {
        m_database->addNewUser(username, password, email);
        return SignedUp;
    }
    return UserAlreadyExists;
}

int LoginManager::login(std::string username, std::string password)
{
    std::lock_guard<std::mutex> lock(m_database_mutex);
    if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password))
    {
        m_LoggedUsers.push_back(LoggedUser(username));
        return LoggedIn;
    }
    return FailedLogin;
}

int LoginManager::logout(std::string username)
{
    std::lock_guard<std::mutex> lock(m_database_mutex);
    for (auto it = m_LoggedUsers.begin(); it != m_LoggedUsers.end(); ++it)
    {
        if (it->getUsername() == username)
        {
            m_LoggedUsers.erase(it);
            return LoggedOut;
        }
    }
    return NoSuchLoggedUser;
}