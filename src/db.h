#ifndef DB_H
#define DB_H

#include <sqlite3.h>

extern sqlite3 *db;

int openDB();
void closeDB();

#endif // DB_H
int initDBSchema(const char *sqlFilePath);
int getUserIdByName(const char *username);
void listUserAccounts(int user_id);
