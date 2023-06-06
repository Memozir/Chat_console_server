#include "Protocol.h"

//void size_last_entity(std::vector<std::string>& entities)
//{
//	int last_index = entities.size() - 1;
//	int last_size = entities.at(last_index).size();
//	auto it = entities.end();
//	entities.insert(it - 1, std::to_string(last_size));
//}
//
//void fix_las_entity(std::vector<std::string>& entities)
//{
//	int last_size = atoi(entities.at(entities.size() - 2).c_str());
//	std::string to_fix = entities.at(entities.size() - 1);
//	//entities.erase(entities.end());
//	//entities.erase(entities.end());
//	entities.pop_back();
//	entities.pop_back();
//
//	std::string fixed;
//
//	for (int i = 0; i < last_size; i++)
//	{
//		fixed += to_fix.at(i);
//	}
//
//	entities.push_back(fixed);
//}

Protocol::Protocol(std::string request)
{
	RequestParser parser(request);
	this->request = parser.parse();
}

std::string Protocol::generate_response(std::vector<std::string> db_result)
{
	std::string response;
	int size = db_result.size();

	if (size == 0)
	{
		response = "0";
		return response;
	}

	for (int i = 0; i < size; i++)
	{
		response += db_result.at(i);

		if (i != size - 1)
			response += '/';
	}

	return response;
}

std::vector<std::string> Protocol::registration(std::vector<std::string> entities)
{
	Db* db = new Db;

	std::string password;
	//fix_las_entity(entities);

	std::cout << "\nCORRECT PASSs: " << password << std::endl;
	std::vector<std::string> result = db->registrate(entities.at(0), entities.at(1));
	delete db;
	return result;
}
std::vector<std::string> Protocol::auth(std::vector<std::string> entities, std::vector<std::string> &users_online)
{
	Db* db = new Db;

	//fix_las_entity(entities);

	std::vector<std::string> result = db->auth(entities.at(0), entities.at(1));
	delete db;

	if (result.at(1) == "1")
	{
		users_online.push_back(entities.at(0));
	}

	return result;
}

std::vector<std::string> Protocol::send_message(std::vector<std::string> entities)
{
	Db* db = new Db;
	//fix_las_entity(entities);
	std::vector<std::string> users;

	for (int i = 2; i < entities.size(); i++)
	{
		users.push_back(entities.at(i));
	}
	std::vector<std::string> result = db->add_message(entities.at(0), users, entities.at(1));
	delete db;
	return result;
}
std::vector<std::string> Protocol::get_user_msg(std::vector<std::string> entities)
{
	Db* db = new Db;
	//fix_las_entity(entities);
	std::vector<std::string> result = db->get_messages_from_user(entities.at(0), entities.at(1));
	delete db;
	return result;
}

std::vector<std::string> Protocol::message_count(std::vector<std::string> entities)
{
	Db* db = new Db;
	//fix_las_entity(entities);
	std::vector<std::string> result = db->message_count(entities.at(0));
	delete db;
	return result;
}

std::string Protocol::response(std::vector<std::string>& users_online)
{
	std::vector<std::string> result;

	switch ((Operations)request.code)
	{
	case Operations::REGISTRATION:
		result = registration(request.entities);
		break;
	case Operations::AUTH:
		result = auth(request.entities, users_online);
		break;
	case Operations::MESSAGE_LIST:
		result = message_count(request.entities);
		break;
	case Operations::USER_LIST:
		result = users_online;
		break;
	case Operations::USER_MESSAGE:
		result = get_user_msg(request.entities);
		break;
	case Operations::SEND_MESSAGE:
		result = send_message(request.entities);
		break;
	case Operations::DISCONNECT:
		std::string dissconnect = "0";
		return dissconnect;
		break;
	}

	return generate_response(result);
}
