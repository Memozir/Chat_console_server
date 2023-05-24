#include "RequestParser.h"

RequestParser::RequestParser(std::string request)
{
	this->request = request;
}

int RequestParser::get_code()
{
	std::string str_code = entities.at(0);
	entities.pop_back();

	int code = atoi(str_code.c_str());

	return code;
}

void RequestParser::print_parsed()
{
	int size = entities.size();
	for (int i = 0; i < size; i++)
	{
		std::cout << entities.at(i) << std::endl;
	}
}

RequestParser::Request* RequestParser::parse()
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

	Request* request = new Request{
		get_code(),
		entities
	};

	return request;
}
