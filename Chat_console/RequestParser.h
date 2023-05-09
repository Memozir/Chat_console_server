#pragma once
#include <iostream>
#include <vector>

class RequestParser
{
private:
	std::string request;
	
	//struct Request {
	//	int entity_count;
	//	std::string* entities;
	//};

	std::vector<std::string> entities;

	//Request* request_entity;

public:
	RequestParser(std::string request);
	//~RequestParser();

	void parse();
	void print_parsed();
};

