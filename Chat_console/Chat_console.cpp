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
	Server server;
	server.init();
	server.socket_init(argc, argv);
	
	while (true)
	{
		server.recv_request();
	}

	server.shutdown_connection();
	server.clean();

}

