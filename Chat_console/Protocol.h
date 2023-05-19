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
		USER_LIST,
		MESSAGE_LIST,
		USER_MESSAGE,
		SEND_MESSAGE,
		DISCONNECT
	};	
};

