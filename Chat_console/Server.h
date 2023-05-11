#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

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

};
