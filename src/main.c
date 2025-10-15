#include "header.h"
#include <sqlite3.h>
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global DB pointer
sqlite3 *db = NULL;

void mainMenu(struct User u)
{
    int option;
    int choice;
    do {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- Check list of owned accounts\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove existing account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Exit\n");
        printf("\nEnter option: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n'); // flush invalid input
            printf("Invalid input. Try again.\n");
            continue;
        }
        while (getchar() != '\n'); // flush input buffer

        switch (option)
        {
            case 1:
                createNewAcc(db, u);
                break;
            case 2:
                updateAccountInfo(db, u);
                break;
            case 3:
                checkAccountDetails(db, u);
                break;
            case 4:
                checkAllAccounts(db, u);
                break;
            case 5:
                makeTransaction(db, u);
                break;
            case 6:
                removeAccount(db, u);
                break;
            case 7:
                transferOwnership(db, u);
                break;
            case 8:
                printf("Exiting...\n");
                closeDB();
                exit(0);
                break;
            default:
                printf("Invalid operation!\n");
                continue;
        }

        // Navigation prompt right here:
        printf("\nPress 1 to return to Main Menu, or 0 to Exit: ");
        while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1)) {
            while (getchar() != '\n'); // flush invalid input
            printf("Invalid input. Please press 1 for Main Menu or 0 to Exit: ");
        }
        while (getchar() != '\n'); // flush input buffer
        if (choice == 0) {
            printf("Exiting...\n");
            closeDB();
            exit(0);
        }
        // if choice == 1, loop continues, showing main menu again
    } while (1);
}
void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM MANAGEMENT SYSTEM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!\n");
                u->id = getUserIdByName(u->name);  // 🔐 Retrieve user_id
                printf("Debug: Logged-in user ID = %d\n", u->id);
                if (u->id == -1) {
                    printf("\nFailed to retrieve user ID.\n");
                    exit(1);
                }
                r = 1;
            }
            else
            {
                printf("\nWrong password or User Name\n");
                exit(1);
            }
            break;

        case 2:
    if (registerMenu(u->name, u->password) == 0) {
        u->id = getUserIdByName(u->name);
        if (u->id == -1) {
            printf("\nFailed to retrieve user ID.\n");
            exit(1);
        }
        r = 1;  // registration succeeded, exit menu loop
    } else {
        // registration failed, don't proceed to login
        // maybe prompt again or just continue loop
        printf("\nPlease try registering again with a different username.\n");
    }
    break;

        case 3:
            exit(0);
            break;

        default:
            printf("Insert a valid operation!\n");
            break;
        }
    }
}

int main()
{
    // Open SQLite DB connection
    if (openDB() != 0) 
    {
        fprintf(stderr, "Failed to open database\n");
        return 1;
    }

    // Initialize DB schema
    if (initDBSchema("data.sql") != 0)
    {
        fprintf(stderr, "Failed to initialize database schema\n");
        closeDB();
        return 1;
    }

    struct User u;
    initMenu(&u);
    mainMenu(u);

    closeDB();
    return 0;
}
