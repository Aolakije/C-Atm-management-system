#include "db.h"
#include <stdio.h>
#include <stdlib.h>   // <-- this one is essential for malloc/free
#include <sqlite3.h>

sqlite3 *db = NULL;

int openDB() {
    int rc = sqlite3_open("atm.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return 0;
}

void closeDB() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

int initDBSchema(const char *sqlFilePath) {
    FILE *f = fopen(sqlFilePath, "r");
    if (!f) {
        perror("Failed to open SQL file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *sql = malloc(len + 1);
    fread(sql, 1, len, f);
    sql[len] = '\0';
    fclose(f);

    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        free(sql);
        return rc;
    }

    free(sql);
    return 0;
}