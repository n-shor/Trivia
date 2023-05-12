#include "LoginManager.h"
#include "LoggedUser.h"

LoginManager::LoginManager()
{
    m_database = new SqliteDatabase();
    m_database->open();
}

int LoginManager::signup(std::string username, std::string password, std::string email)
{
    if (!m_database->doesUserExist(username)) {
        m_database->addNewUser(username, password, email);
        return SignedUp;
    }
    return UserAlreadyExists;
    // Add error handling or other state changes as needed
}

int LoginManager::login(std::string username, std::string password)
{
    if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password))
    {
        m_LoggedUsers.push_back(*(new LoggedUser(username)));
        return LoggedIn;
    }
    return FailedLogin;
}

int LoginManager::logout(std::string username)
{
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

LoginManager::~LoginManager()
{
    m_database->close();
    delete m_database;
}
