#ifndef HEADER_H
#define HEADER_H
#include <sqlite3.h>


struct Date
{
    int month, day, year;
};

struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    char phone[20];        // changed from int to string
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system functions
void createNewAcc(sqlite3 *db, struct User u);
void checkAllAccounts(sqlite3 *db, struct User u);
void updateAccountInfo(sqlite3 *db, struct User u);
void checkAccountDetails(sqlite3 *db, struct User u);
void makeTransaction(sqlite3 *db, struct User u);
void removeAccount(sqlite3 *db, struct User u);
void transferOwnership(sqlite3 *db, struct User u);


#endif // HEADER_H
