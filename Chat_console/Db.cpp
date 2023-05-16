#include "Db.h"

Db::Db()
{
	char* err = 0;

	// ��������� ����������
	if (sqlite3_open("project_db.dblite", &db))
		fprintf(stderr, "Error openning/creating DB: %s\n", sqlite3_errmsg(db));

	int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS User(username varchar(30), password varchar(20));", NULL, NULL, &err);

	// ��������� SQL
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Error SQL: %sn", err);
		sqlite3_free(err);
	}
	// ��������� ����������
}

Db::~Db()
{
	sqlite3_close(db);
}
