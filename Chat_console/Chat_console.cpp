// Chat_console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <conio.h>
#include <locale.h>

#include "RequestParser.h"
#include "Server.h"

int main()
{
	RequestParser parser("10/mojo/123");
	RequestParser::Request req =  parser.parse();

}

