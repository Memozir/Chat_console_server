#pragma once

#include "RequestParser.h"
#include "Db.h"
#include "Server.h"

class Protocol
{
private:
	RequestParser::Request request;

	enum class Operations
	{
		REGISTRATION,
		AUTH,
		USER_LIST,
		MESSAGE_LIST,
		USER_MESSAGE,
		SEND_MESSAGE,
		DISCONNECT
	};

	std::string generate_response(std::vector<std::string> db_result);

public:
	Protocol(std::string request);

	std::string response(std::vector<std::string> &users_online);
	std::vector<std::string> registration(std::vector<std::string>);
	std::vector<std::string> auth(std::vector<std::string>, std::vector<std::string> &users_online);
	std::vector<std::string> send_message(std::vector<std::string>);
	std::vector<std::string> get_user_msg(std::vector<std::string>);
	std::vector<std::string> message_count(std::vector<std::string>);

};

