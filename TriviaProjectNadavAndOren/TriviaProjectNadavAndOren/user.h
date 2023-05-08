#pragma once
#include <string>
class user 
{
private:
	std::string _username;
	std::string _email;
	std::string _password;
public:
	user(std::string username, std::string email, std::string password);
	~user();
	std::string getUsername();
	std::string getEmail();
	std::string getPassword();
};