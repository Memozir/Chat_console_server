// Chat_console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <conio.h>
#include <locale.h>
#include "sqlite3.h"

#include "RequestParser.h"
#include "Server.h"
#include "Db.h"

int __cdecl main(int& argc, char** argv)
{
	//RequestParser parser("10/mojo/123");
	//RequestParser::Request req =  parser.parse();
	//std::cout << req.entities.at(0);

	Db* db = new Db();
	//db->registrate("Yura", "12345");
	//db->registrate("Alex", "54321");
	std::vector<std::string> users;
	users.push_back("Alex");
	db->add_message("Yura", users, "Everybody HI!");

	//std::cout << "AUTH: " << db->auth("Alex", "54321") << std::endl;

	delete db;
	//Server server;
	//server.init();
	//server.socket_init(argc, argv);
	//server.recv_request();
	//server.shutdown_connection();
	//server.clean();

}

