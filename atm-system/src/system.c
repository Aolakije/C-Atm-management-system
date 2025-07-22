#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "header.h"

int getAccountFromDB(sqlite3 *db, int accountId, char name[50], struct Record *r)
{
    const char *sql = "SELECT accounts.id, accounts.user_id, users.name, accounts.account_id, "
                      "accounts.creation_date, accounts.country, accounts.phone_number, "
                      "accounts.balance, accounts.account_type "
                      "FROM accounts "
                      "JOIN users ON users.user_id = accounts.user_id "
                      "WHERE accounts.account_id = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, accountId);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        r->id = sqlite3_column_int(stmt, 0);
        r->userId = sqlite3_column_int(stmt, 1);

        const unsigned char *userName = sqlite3_column_text(stmt, 2);
        if (userName)
            strncpy(name, (const char *)userName, 49);
        name[49] = '\0';

        r->accountNbr = sqlite3_column_int(stmt, 3);

        const unsigned char *creationDate = sqlite3_column_text(stmt, 4);
        if (creationDate)
        {
            int year, month, day;
            sscanf((const char *)creationDate, "%d-%d-%d", &year, &month, &day);
            r->deposit.year = year;
            r->deposit.month = month;
            r->deposit.day = day;
        }

        const unsigned char *country = sqlite3_column_text(stmt, 5);
        if (country)
            strncpy(r->country, (const char *)country, sizeof(r->country) - 1);
        r->country[sizeof(r->country) - 1] = '\0';

        const unsigned char *phone = sqlite3_column_text(stmt, 6);
        if (phone)
            strncpy(r->phone, (const char *)phone, sizeof(r->phone) - 1);
        r->phone[sizeof(r->phone) - 1] = '\0';

        r->amount = sqlite3_column_double(stmt, 7);

        const unsigned char *accountType = sqlite3_column_text(stmt, 8);
        if (accountType)
            strncpy(r->accountType, (const char *)accountType, sizeof(r->accountType) - 1);
        r->accountType[sizeof(r->accountType) - 1] = '\0';

        sqlite3_finalize(stmt);
        return 1;
    }
    else
    {
        sqlite3_finalize(stmt);
        return 0;
    }
}
int saveAccountToDB(sqlite3 *db, struct User u, struct Record r)
{
    const char *sql = "UPDATE accounts SET "
                      "user_id = ?, account_id = ?, creation_date = ?, country = ?, "
                      "phone_number = ?, balance = ?, account_type = ? "
                      "WHERE id = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Failed to prepare update statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, u.id);
    sqlite3_bind_int(stmt, 2, r.accountNbr);

    char creationDate[20];
    snprintf(creationDate, sizeof(creationDate), "%04d-%02d-%02d",
             r.deposit.year, r.deposit.month, r.deposit.day);
    sqlite3_bind_text(stmt, 3, creationDate, -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4, r.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, r.phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, r.amount);
    sqlite3_bind_text(stmt, 7, r.accountType, -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, 8, r.id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Failed to update account: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

#include <sqlite3.h>

void createNewAcc(sqlite3 *db, struct User u) {
    struct Record r;
    memset(&r, 0, sizeof(r));

    printf("Enter account number: ");
    scanf("%d", &r.accountNbr);

    printf("Enter creation date (MM DD YYYY): ");
    scanf("%d %d %d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    printf("Enter country: ");
    scanf("%s", r.country);

    printf("Enter phone number: ");
    scanf("%s", r.phone);

    printf("Enter initial balance: ");
    scanf("%lf", &r.amount);

    printf("Enter account type: ");
    scanf("%s", r.accountType);

    const char *sql = "INSERT INTO accounts "
                      "(user_id, username, account_id, creation_date, country, phone_number, balance, account_type) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";  // 8 parameters

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare insert statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    char creationDate[20];
    snprintf(creationDate, sizeof(creationDate), "%04d-%02d-%02d",
             r.deposit.year, r.deposit.month, r.deposit.day);

    // Correct parameter bindings
    sqlite3_bind_int(stmt, 1, u.id);                                // user_id
    sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);          // username
    sqlite3_bind_int(stmt, 3, r.accountNbr);                        // account_id
    sqlite3_bind_text(stmt, 4, creationDate, -1, SQLITE_STATIC);    // creation_date
    sqlite3_bind_text(stmt, 5, r.country, -1, SQLITE_STATIC);       // country
    sqlite3_bind_text(stmt, 6, r.phone, -1, SQLITE_STATIC);         // phone_number
    sqlite3_bind_double(stmt, 7, r.amount);                         // balance
    sqlite3_bind_text(stmt, 8, r.accountType, -1, SQLITE_STATIC);   // account_type

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Failed to insert account: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Account created successfully!\n");
    }

    sqlite3_finalize(stmt);
}

void checkAllAccounts(sqlite3 *db, struct User u) {
    const char *sql =
        "SELECT accounts.account_id, accounts.creation_date, accounts.balance, accounts.account_type "
        "FROM accounts WHERE user_id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, u.id);

    printf("Accounts for user %s:\n", u.name);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int accountId = sqlite3_column_int(stmt, 0);
        const unsigned char *creationDate = sqlite3_column_text(stmt, 1);
        double balance = sqlite3_column_double(stmt, 2);
        const unsigned char *accountType = sqlite3_column_text(stmt, 3);

        printf("Account ID: %d\n", accountId);
        printf("Creation Date: %s\n", creationDate ? (const char *)creationDate : "N/A");
        printf("Balance: %.2lf\n", balance);
        printf("Type: %s\n\n", accountType ? (const char *)accountType : "N/A");
    }

    sqlite3_finalize(stmt);
}
void updateAccountInfo(sqlite3 *db, struct User u) {
    int accountId;
    printf("Enter account ID to update: ");
    scanf("%d", &accountId);

    struct Record r;
    char userName[100];
    if (!getAccountFromDB(db, accountId, userName, &r)) {
        printf("Account not found.\n");
        return;
    }

    printf("Updating account %d (current balance: %.2lf)\n", accountId, r.amount);

    printf("Enter new country (current: %s): ", r.country);
    scanf("%s", r.country);

    printf("Enter new phone number (current: %s): ", r.phone);
    scanf("%s", r.phone);

    printf("Enter new account type (current: %s): ", r.accountType);
    scanf("%s", r.accountType);

    printf("Enter new balance (current: %.2lf): ", r.amount);
    scanf("%lf", &r.amount);

    // Save updated record
    if (saveAccountToDB(db, u, r))
        printf("Account updated successfully.\n");
    else
        printf("Failed to update account.\n");
}
void checkAccountDetails(sqlite3 *db, struct User u) {
    int accountId;
    printf("Enter account ID to view details: ");
    scanf("%d", &accountId);

    struct Record r;
    char userName[100];
    if (!getAccountFromDB(db, accountId, userName, &r)) {
        printf("Account not found.\n");
        return;
    }

    printf("Account Details:\n");
    printf("User: %s\n", userName);
    printf("Account Number: %d\n", r.accountNbr);
    printf("Creation Date: %02d/%02d/%04d\n", r.deposit.month, r.deposit.day, r.deposit.year);
    printf("Country: %s\n", r.country);
    printf("Phone: %s\n", r.phone);
    printf("Balance: %.2lf\n", r.amount);
    printf("Account Type: %s\n", r.accountType);
}
void makeTransaction(sqlite3 *db, struct User u) {
    int accountId;
    printf("Enter account ID for transaction: ");
    scanf("%d", &accountId);

    struct Record r;
    char userName[100];
    if (!getAccountFromDB(db, accountId, userName, &r)) {
        printf("Account not found.\n");
        return;
    }

    printf("Current balance: %.2lf\n", r.amount);
    printf("Enter 1 for deposit, 2 for withdrawal: ");
    int choice;
    scanf("%d", &choice);

    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (choice == 1) {
        r.amount += amount;
        printf("Deposited %.2lf\n", amount);
    } else if (choice == 2) {
        if (amount > r.amount) {
            printf("Insufficient funds.\n");
            return;
        }
        r.amount -= amount;
        printf("Withdrew %.2lf\n", amount);
    } else {
        printf("Invalid choice.\n");
        return;
    }

    if (saveAccountToDB(db, u, r))
        printf("Transaction successful. New balance: %.2lf\n", r.amount);
    else
        printf("Failed to update account.\n");
}
void removeAccount(sqlite3 *db, struct User u) {
    int accountId;
    printf("Enter account ID to remove: ");
    scanf("%d", &accountId);

    // Confirm account belongs to user
    struct Record r;
    char userName[100];
    if (!getAccountFromDB(db, accountId, userName, &r)) {
        printf("Account not found.\n");
        return;
    }

    if (r.userId != u.id) {
        printf("You do not have permission to delete this account.\n");
        return;
    }

    const char *sql = "DELETE FROM accounts WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare delete statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, r.id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Failed to delete account: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Account deleted successfully.\n");
    }

    sqlite3_finalize(stmt);
}
void transferOwnership(sqlite3 *db, struct User u) {
    int accountId, newUserId;
    printf("Enter your ACCOUNT ID: ");
    scanf("%d", &accountId);

    struct Record r;
    char currentUsername[100];
    if (!getAccountFromDB(db, accountId, currentUsername, &r)) {
        printf("Account not found.\n");
        return;
    }

    if (r.userId != u.id) {
        printf("You do not own this account.\n");
        return;
    }

    printf("Enter NEW USER_ID to transfer ownership to: ");
    scanf("%d", &newUserId);

    // Step 1: Get the new owner's username
    const char *sqlGetName = "SELECT name FROM users WHERE user_id = ?;";
    sqlite3_stmt *stmtName;
    if (sqlite3_prepare_v2(db, sqlGetName, -1, &stmtName, NULL) != SQLITE_OK) {
        printf("Failed to prepare name query: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmtName, 1, newUserId);

    char newUsername[100];  // Buffer for new username
    if (sqlite3_step(stmtName) == SQLITE_ROW) {
        const unsigned char *nameText = sqlite3_column_text(stmtName, 0);
        snprintf(newUsername, sizeof(newUsername), "%s", nameText);
    } else {
        printf("New user ID not found.\n");
        sqlite3_finalize(stmtName);
        return;
    }
    sqlite3_finalize(stmtName);

    // Step 2: Update both user_id and username in accounts
    const char *sqlUpdate = "UPDATE accounts SET user_id = ?, username = ? WHERE account_id = ?;";
    sqlite3_stmt *stmtUpdate;
    if (sqlite3_prepare_v2(db, sqlUpdate, -1, &stmtUpdate, NULL) != SQLITE_OK) {
        printf("Failed to prepare ownership update: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmtUpdate, 1, newUserId);
    sqlite3_bind_text(stmtUpdate, 2, newUsername, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmtUpdate, 3, accountId);

    if (sqlite3_step(stmtUpdate) != SQLITE_DONE) {
        printf("Failed to transfer ownership: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Ownership transferred successfully to %s (user_id %d).\n", newUsername, newUserId);
    }

    sqlite3_finalize(stmtUpdate);
}
