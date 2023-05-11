#include "Server.h"

//Server::Server() {};
//Server::~Server() {};

void Server::init()
{
	if (FAILED (WSAStartup(MAKEWORD(1, 1), &ws)))
	{
		//int error = WSAGetLastError();
		//std::cout << "WSA START ERROR: " << error << "\n";
	}
}

void Server::socket_init()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == sock)
	{
		//int error = WSAGetLastError();
		//std::cout << "SOCKET INIT ERROR: " << error << "\n";
	}
}
