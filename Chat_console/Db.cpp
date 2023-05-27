#include "Db.h"

Db::Db()
{
	char* err = 0;

	// открываем соединение
	if (sqlite3_open("project_db.dblite", &db))
		fprintf(stderr, "Error openning/creating DB: %s\n", sqlite3_errmsg(db));

	int rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &err);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
	}

	rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS User(user_id integer primary key AUTOINCREMENT, username varchar(30), password varchar(20));", NULL, NULL, &err);

	// выполняем SQL
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
	}
	
	rc = sqlite3_exec(db,
		"CREATE TABLE IF NOT EXISTS Messages(msg_id integer primary key AUTOINCREMENT, from_id integer foreign_key references User(user_id), to_id integer foreign_key references User(user_id), msg_text varchar(512));",
		NULL, NULL, &err);

	// выполняем SQL
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
	}
}

Db::~Db()
{
	sqlite3_close(db);
}

int Db::is_exist(std::string username)
{
	sqlite3_stmt* stmt;

	std::string check_query = "SELECT user_id FROM User WHERE username = '" + username + "';";

	sqlite3_prepare_v2(db, check_query.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);

	int id = sqlite3_column_int(stmt, 0);

	return id;
}

std::vector<std::string>* Db::auth(std::string username, std::string pass)
{
	std::vector<std::string>* result = new std::vector<std::string>;

	sqlite3_stmt* stmt;
	std::string query = "SELECT user_id FROM User WHERE username='" + username + "' AND password='" + pass + "';";
	std::cout << query << std::endl;

	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);

	int check = sqlite3_column_int(stmt, 0);
	std::cout << "CHECK: " << check << std::endl;

	if (check == 0)
	{
		return result;
	}

	result->push_back("1");

	return result;
}

std::vector<std::string>* Db::registrate(std::string username, std::string pass)
{
	std::vector<std::string>* result = new std::vector<std::string>;

	if (is_exist(username) > 0)
	{
		std::cout << "ALREADY EXISTS\n";
		return result;
	}

	std::cout << "NOT EXIST\n";
	char* err;

	std::string query = "INSERT INTO User(username, password) VALUES('" + username + "', '" + pass + "');";

	int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
		return result;
	}

	result->push_back("1");
	return result;
}

int Db::get_id(std::string username)
{
	if (is_exist(username) == 0)
	{
		return -1;
	}

	sqlite3_stmt* stmt;
	std::string query = "SELECT user_id FROM User WHERE username='" + username + "';";
	std::cout << query << std::endl;

	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);

	return sqlite3_column_int(stmt, 0);
}

std::vector<std::string>* Db::add_message(std::string username, std::vector<std::string> users, std::string msg_text)
{
	std::vector<std::string>* result = new std::vector<std::string>;

	char* err;
	int id = get_id(username);

	std::cout << "FROM ID: " << id << std::endl;

	if (id == -1)
	{
		return result;
	}

	for (int i = 0; i < users.size(); i++)
	{
		std::cout << "FROM USER: " << users.at(i) << std::endl;
		int to_id = get_id(users.at(i));
		std::cout << "TO ID: " << to_id << std::endl;

		if (to_id == -1)
		{
			std::cout << "TO_ID NOT EXISTS\n";
			continue;
		}

		std::string query = "INSERT INTO Messages(from_id, to_id, msg_text) VALUES(" + std::to_string(id)+ ", " + std::to_string(to_id) + ", '" + msg_text + "');";
		int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);

		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Error SQL: %sn", err);
			sqlite3_free(err);
			return result;
		}

	}
	users.clear();

	result->push_back("1");

	return result;
}

std::vector<std::string>* Db::get_messages_from_user(std::string username, std::string user_from)
{
	std::vector<std::string>* messages = new std::vector<std::string>;

	int user_id = is_exist(username);

	if (user_id == 0)
	{
		std::cout << "USER DOES NOT EXISTS\n";
		return messages;
	}

	int user_from_id = is_exist(user_from);

	if (user_from_id > 0)
	{
		std::cout << "USER FROM ALREADY EXISTS\n";
		return messages;
	}

	sqlite3_stmt* stmt;
	std::string query = "SELECT msg_text FROM Messages WHERE to_id = " + std::to_string(user_id) + " AND from_id = " + std::to_string(user_from_id) + ";";

	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		const unsigned char* msg = sqlite3_column_text(stmt, 0);
		std::string result = std::string(reinterpret_cast<const char*>(msg));
		messages->push_back(result);
	}
	
	return messages;
}

std::vector<std::string>* Db::message_count(std::string username)
{
	std::vector<std::string>* result = new std::vector<std::string>;

	int user_id = is_exist(username);

	if (user_id == 0)
	{
		std::cout << "USER DOES NOT EXISTS\n";
		return result;
	}

	sqlite3_stmt* stmt;
	std::string sql = "SELECT t2.username, count(*) as mes_count FROM Messages as t1 inner join Users as t2 on t1.from_id = t2.user_id WHERE to_id = '" + std::to_string(user_id) + "' GROUP BY from_id;";
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		const unsigned char* msg = sqlite3_column_text(stmt, 0);
		int count = sqlite3_column_int(stmt, 1);
		std::string res = std::string(reinterpret_cast<const char*>(msg)) + ": " + std::to_string(count);
		result->push_back(res);
	}

	return result;
}
