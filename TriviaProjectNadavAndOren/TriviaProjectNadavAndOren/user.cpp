#include "user.h"

user::user(std::string username, std::string email, std::string password)
{
	_username = username;
	_email = email;
	_password = password;
}

user::~user()
{

}

std::string user::getUsername()
{
	return _username;
}

std::string user::getEmail()
{
	return _email;
}

std::string user::getPassword()
{
	return _password;
}
