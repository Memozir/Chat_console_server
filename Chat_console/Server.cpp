#include "Server.h"

Server::Server()
{
	sock_listen = INVALID_SOCKET;
	sock_client = INVALID_SOCKET;
};
//Server::~Server() {};

void Server::init()
{
	int res = WSAStartup(MAKEWORD(2, 2), &ws);

	if (res != 0)
	{
		int error = WSAGetLastError();
		std::cout << "WSA START ERROR: " << error << "\n";
		exit(1);
	}

	std::cout << "WSA START SUCCESS: " << "\n";
}

void Server::close_listen()
{
	closesocket(sock_listen);
}

void Server::socket_init(int& argc, char** argv)
{
	ZeroMemory(&addr, sizeof(addr));
	addr.ai_family = AF_UNSPEC;
	addr.ai_socktype = SOCK_STREAM;
	addr.ai_protocol = IPPROTO_TCP;

	int res = getaddrinfo(NULL, "27015", &addr, &result);
	if (res != 0)
	{
		std::cout << "Gettaddrinfo failed: " << res << std::endl;
		WSACleanup();
		exit(1);
	}

	std::cout << "Gettaddrinfo SUCCESS" << "\n";

	res = 0;
	sock_listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (sock_listen == INVALID_SOCKET)
	{
		std::cout << "Inavalid socket: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	res = bind(sock_listen, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(sock_listen);
		WSACleanup();
		exit(1);
	}

	freeaddrinfo(result);

	res = listen(sock_listen, SOMAXCONN);
	if (res == SOCKET_ERROR)
	{
		std::cout << "Listen failed: " << WSAGetLastError() << std::endl;
		closesocket(sock_listen);
		WSACleanup();
		exit(1);
	}

	sock_client = accept(sock_listen, NULL, NULL);
	if (sock_client == INVALID_SOCKET)
	{
		std::cout << "Accept failed: " << WSAGetLastError() << std::endl;
		closesocket(sock_client);
		WSACleanup();
		exit(1);
	}
	
}

void Server::send_response(std::string request)
{
	int res = 0;
	std::cout << "\n---SEND RESPONSE---\n" << request.c_str() << "\n---SEND RESPONSE---\n";
	int send_res = send(sock_client, request.c_str(), strlen(request.c_str()), 0);
	if (send_res == SOCKET_ERROR) {
		std::cout << "Send failed: " << WSAGetLastError() << std::endl;
		closesocket(sock_client);
		WSACleanup();
		exit(1);
	}
	std::cout << "Request send: OK" << std::endl;
}

void Server::recv_request()
{
	int res;
	//std::string test = "0/t1/132";

	do
	{
		std::string request;
		std::cout << "Listenning...\n";
		res = recv(sock_client, &request[0], 512, 0);

		if (res > 0)
		{
			std::cout << "\nCHECK: " << request.c_str() << std::endl;
			//std::string new_request = request;

			Protocol prot(request);
			std::string response = prot.response(users_online);

			send_response(response);
			std::cout << "Recieved: " << res << " bytes" << std::endl;
		}
		else if (res == 0)
			printf("Connection closing...\n");
		else
		{
			std::cout << "Recieve failed: " << WSAGetLastError() << std::endl;
			closesocket(sock_client);
			WSACleanup();
			exit(1);
		}
	} while (true);
}

void Server::shutdown_connection()
{
	int res = shutdown(sock_client, SD_BOTH);
	if (res == SOCKET_ERROR) {
		std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
		closesocket(sock_client);
		WSACleanup();
		exit(1);
	}
}

void Server::clean()
{
	closesocket(sock_client);
	WSACleanup();
}

void Server::set_online(std::string username)
{
	users_online.push_back(username);
}
