#include "RequestParser.h"

RequestParser::RequestParser(std::string request)
{
	this->request = request;
}

void RequestParser::print_parsed()
{
	int size = entities.size();
	for (int i = 0; i < size; i++)
	{
		std::cout << entities.at(i) << std::endl;
	}
}

void RequestParser::parse()
{
	int request_len = this->request.length();
	std::string buf;

	for (int i = 0; i < request_len; i++)
	{
		if (request[i] != '/')
		{
			buf += request[i];
		}
		else
		{
			entities.push_back(buf);
			buf.clear();
		}
	}
	entities.push_back(buf);
}
