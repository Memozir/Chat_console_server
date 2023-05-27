#include "Protocol.h"

Protocol::Protocol(std::string request)
{
	RequestParser parser(request);
	this->request = parser.parse();
}

std::string Protocol::generate_response(std::vector<std::string>* db_result)
{
	std::string response;
	int size = db_result->size();

	if (size == 0)
	{
		response = "0";
		return response;
	}

	for (int i = 0; i < size; i++)
	{
		response += db_result->at(i);

		if (i != size - 1)
			response += '/';
	}

	return response;
}

std::vector<std::string>* Protocol::registration(std::vector<std::string> entities)
{
	Db db;
	return db.registrate(entities.at(0), entities.at(1));
}
std::vector<std::string>* Protocol::auth(std::vector<std::string> entities)
{
	Db db;
	return db.auth(entities.at(0), entities.at(1));
}
//std::vector<std::string*> Protocol::user_list(std::vector<std::string> entities)
//{
//	Db db;
//	return db.auth(entities.at(0), entities.at(1));
//}
std::vector<std::string>* Protocol::send_message(std::vector<std::string> entities)
{
	Db db;
	std::vector<std::string> users;

	for (int i = 2; i < entities.size(); i++)
	{
		users.push_back(entities.at(i));
	}

	return db.add_message(entities.at(0), users, entities.at(1));
}
std::vector<std::string>* Protocol::get_user_msg(std::vector<std::string> entities)
{
	Db db;
	return db.get_messages_from_user(entities.at(0), entities.at(1));
}

std::vector<std::string>* Protocol::message_count(std::vector<std::string> entities)
{
	Db db;
	return db.message_count(entities.at(0));
}

std::string Protocol::response(std::vector<std::string> users)
{
	std::vector<std::string>* result;

	switch ((Operations)request->code)
	{
	case Operations::REGISTRATION:
		result = registration(request->entities);
		break;
	case Operations::AUTH:
		result = auth(request->entities);
		break;
	case Operations::MESSAGE_LIST:
		result = message_count(request->entities);
		break;
	case Operations::USER_LIST:
		result = &users;
		break;
	case Operations::USER_MESSAGE:
		result = get_user_msg(request->entities);
		break;
	case Operations::SEND_MESSAGE:
		result = send_message(request->entities);
		break;
	case Operations::DISCONNECT:
		std::string dissconnect = "0";
		return dissconnect;
		break;
	}

	return generate_response(result);
}
