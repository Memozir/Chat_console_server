#pragma once

#include "RequestParser.h"

class Protocol
{
public:
	Protocol(RequestParser::Request entity);
	//~Protocol();

private:
	enum class Operations
	{
		REGISTRATION,
		AUTH,
		USERS_LIST,
		MESSAGE_LIST
	};	
};

