#pragma once
#include "sqlite3.h"
#include <iostream>

class Db
{
private:
	sqlite3* db;
	sqlite3_stmt* stmt;
public:
	Db();
	~Db();
};

