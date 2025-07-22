#include "db.h"
#include <stdio.h>
#include <stdlib.h>   // <-- this one is essential for malloc/free
#include <sqlite3.h>
#include <string.h>

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

int loginUser(const char *name, const char *password) {
    const char *sql = "SELECT password FROM users WHERE name = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *db_pass = sqlite3_column_text(stmt, 0);
        if (strcmp((const char *)db_pass, password) == 0) {
            result = 1;  // password matches
        }
    }

    sqlite3_finalize(stmt);
    return result;
}
// Add this to db.c

int getUserIdByName(const char *username) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT user_id FROM users WHERE name = ?";
    int user_id = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Prepare error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return user_id;
}
