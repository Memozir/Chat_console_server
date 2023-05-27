#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>

#include "Protocol.h"

#pragma comment(lib, "Ws2_32.lib")

class Server
{
public:

	Server();
	//~Server();

	void init();
	void socket_init(int& argc, char** argv);
	void close_listen();
	void send_response(std::string request);
	void recv_request();
	void shutdown_connection();
	void clean();
	void set_online(std::string username);

private:
	WSADATA ws;
	SOCKET sock_listen;
	SOCKET sock_client;
	addrinfo addr, *result;
	
	std::vector<std::string>* users_online = new std::vector<std::string>;
};
