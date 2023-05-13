#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

class Server
{
public:

	Server();
	~Server();

	void init();
	void socket_init();
	void close();

private:
	WSADATA ws;
	SOCKET sock;
	addrinfo addr;
	
	std::map<std::string, bool> auth;
	std::map<std::string, bool>::iterator auth_it = auth.begin();
};
