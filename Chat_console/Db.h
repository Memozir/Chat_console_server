#pragma once
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <string>

class Db
{
private:
	sqlite3* db;

	int get_id(std::string username);

public:
	Db();
	~Db();
	
	std::vector<std::string>* registrate(std::string username, std::string pass);
	std::vector<std::string>* auth(std::string username, std::string pass);
	int is_exist(std::string username);
	std::vector<std::string>* add_message(std::string username, std::vector<std::string> users, std::string msg_text);
	std::vector<std::string>* message_count(std::string username);
	std::vector<std::string>* get_messages_from_user(std::string username, std::string user_from);
};

