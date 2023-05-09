// Chat_console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include "RequestParser.h"

int main()
{
    RequestParser parser("test1/test2/test3");
    parser.parse();
    parser.print_parsed();
}

