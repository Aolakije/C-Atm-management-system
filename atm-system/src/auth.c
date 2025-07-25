#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sqlite3.h>
#include "header.h"
#include "db.h"
#include <ctype.h>


void loginMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
}

const char *getPassword(struct User u) {
    static char foundPassword[50];
    sqlite3_stmt *stmt;
    const char *sql = "SELECT password FROM users WHERE name = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
        return "error";
    }

    sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        strncpy(foundPassword, (const char *)sqlite3_column_text(stmt, 0), sizeof(foundPassword));
        sqlite3_finalize(stmt);
        return foundPassword;
    }

    sqlite3_finalize(stmt);
    return "no user found";
}
// Helper function to convert strings to lowercase
void toLower(char *str) {
    for (; *str; ++str)
        *str = tolower(*str);
}

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sqlite3.h>

// Helper to validate date format YYYY-MM-DD (basic check)
int validDateFormat(const char *date) {
    int y, m, d;
    if (sscanf(date, "%4d-%2d-%2d", &m, &d, &y) != 3) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0; // Simplified day check
    return 1;
}

int registerMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;
    sqlite3_stmt *stmt;
    int userExists = 0;
    int c;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");

    // Get username
    printf("\n\n\t\t\t\tEnter username: ");
    scanf("%49s", a);
    while ((c = getchar()) != '\n' && c != EOF);  // flush stdin

    // Check if user already exists
    const char *checkSql = "SELECT COUNT(*) FROM users WHERE name = ?";
    if (sqlite3_prepare_v2(db, checkSql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sqlite3_bind_text(stmt, 1, a, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count > 0) userExists = 1;
    }
    sqlite3_finalize(stmt);

    if (userExists) {
        printf("\n\n\t\t\t\tUser already exists! Please choose a different username.\n");
        printf("\n\n\t\t\t\tPress Enter to continue...");
        getchar();
        return 1;  // Fail registration
    }

    // Get password with hidden input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        return 1;
    }

    printf("\n\n\t\t\t\tEnter password: ");
    scanf("%49s", pass);
    while ((c = getchar()) != '\n' && c != EOF);  // flush stdin

    // Restore terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        return 1;
    }

    // Insert new user into database
    const char *insertUserSql = "INSERT INTO users (name, password) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insertUserSql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sqlite3_bind_text(stmt, 1, a, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "SQL insert error (user): %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }
    sqlite3_finalize(stmt);

    // Get the new user's ID
    sqlite3_int64 user_id = sqlite3_last_insert_rowid(db);

    // Prompt for account details
    char accountName[50], date[20], country[50], phone[20], accountType[20];
    double balance;
    int accountId;

    printf("\n\n\t\t\t\tEnter account name: ");
    scanf("%49s", accountName);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n\t\t\t\tEnter account ID (number): ");
    scanf("%d", &accountId);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n\t\t\t\tEnter date (MM-DD-YYYY): ");
    scanf("%19s", date);
    while ((c = getchar()) != '\n' && c != EOF);

    // ** New date validation check **
    if (!validDateFormat(date)) {
        printf("\n\t\t\t\tInvalid date format. Use MM-DD-YYYY. Registration aborted.\n");
        printf("\n\t\t\t\tPress Enter to continue...");
        getchar();
        return 1;
    }
    printf("\n\t\t\t\tEnter country: ");
    scanf("%49s", country);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n\t\t\t\tEnter phone: ");
    scanf("%19s", phone);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n\t\t\t\tChoose account type (savings, fixed01, fixed02, fixed03, current): ");
    scanf("%19s", accountType);
    while ((c = getchar()) != '\n' && c != EOF);

    // Validate accountType against allowed list
    if (strcmp(accountType, "savings") != 0 &&
        strcmp(accountType, "fixed01") != 0 &&
        strcmp(accountType, "fixed02") != 0 &&
        strcmp(accountType, "fixed03") != 0 &&
        strcmp(accountType, "current") != 0) {
        printf("\n\t\t\t\tInvalid account type entered. Registration aborted.\n");
        printf("\n\t\t\t\tPress Enter to continue...");
        getchar();
        return 1;
    }

    printf("\n\t\t\t\tEnter initial balance: ");
    scanf("%lf", &balance);
    while ((c = getchar()) != '\n' && c != EOF);

    // Insert account record
    const char *insertAccountSql =
        "INSERT INTO accounts (user_id, username, account_id, creation_date, country, phone_number, balance, account_type) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, insertAccountSql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error (account): %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, accountName, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, accountId);
    sqlite3_bind_text(stmt, 4, date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 7, balance);
    sqlite3_bind_text(stmt, 8, accountType, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "SQL insert error (account): %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);

    printf("\n\n\t\t\t\tRegistration successful! You can now login.\n");
    printf("\n\n\t\t\t\tPress Enter to continue...");
    getchar();

    return 0;  // success
}
