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
	
	void registrate(std::string username, std::string pass);
	bool auth(std::string username, std::string pass);
	int is_exist(std::string username);
	void add_message(std::string username, std::vector<std::string> users, std::string msg_text);
	int message_count(std::string username);
	std::vector<const unsigned char*>& get_messages_from_user(std::string username, std::string user_from);
};

