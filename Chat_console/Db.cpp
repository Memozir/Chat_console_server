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

bool Db::auth(std::string username, std::string pass)
{
	sqlite3_stmt* stmt;
	std::string query = "SELECT user_id FROM User WHERE username='" + username + "' AND password='" + pass + "';";
	std::cout << query << std::endl;

	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);

	int check = sqlite3_column_int(stmt, 0);
	std::cout << "CHECK: " << check << std::endl;

	if (check == 0)
	{
		return false;
	}

	return true;
}

void Db::registrate(std::string username, std::string pass)
{
	if (is_exist(username) > 0)
	{
		std::cout << "ALREADY EXISTS\n";
		return;
	}

	std::cout << "NOT EXIST\n";
	char* err;

	std::string query = "INSERT INTO User(username, password) VALUES('" + username + "', '" + pass + "');";

	int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
	}

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

void Db::add_message(std::string username, std::vector<std::string> users, std::string msg_text)
{
	char* err;
	int id = get_id(username);

	std::cout << "FROM ID: " << id << std::endl;

	if (id == -1)
	{
		return;
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
		}

	}
	users.clear();
}

std::vector<const unsigned char*>& Db::get_messages_from_user(std::string username, std::string user_from)
{
	std::vector<const unsigned char*>* messages = new std::vector<const unsigned char*>;

	int user_id = is_exist(username);

	if (user_id > 0)
	{
		std::cout << "ALREADY EXISTS\n";
		return *messages;
	}

	int user_from_id = is_exist(user_from);

	if (user_from_id > 0)
	{
		std::cout << "ALREADY EXISTS\n";
		return *messages;
	}

	sqlite3_stmt* stmt;
	std::string query = "SELECT msg_text from Messages WHERE to_id = " + std::to_string(user_id) + " AND from_id = " + std::to_string(user_from_id) + ";";

	sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		const unsigned char* msg = sqlite3_column_text(stmt, 0);
		messages->push_back(msg);
	}
	
	return *messages;
}
