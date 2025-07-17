#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    static struct User userChecker;  // Made static to persist after function returns

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            return userChecker.password;
        }
    }

    fclose(fp);
    return "no user found";
}

void registerMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;
    FILE *fp;
    struct User userChecker;
    int userExists = 0;
    int nextId = 1;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");
    
    // Get username
    printf("\n\n\t\t\t\tEnter username: ");
    scanf("%s", a);

    // Check if user already exists and get next ID
    if ((fp = fopen("./data/users.txt", "r")) != NULL)
    {
        while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
        {
            if (strcmp(userChecker.name, a) == 0)
            {
                userExists = 1;
                break;
            }
            if (userChecker.id >= nextId)
            {
                nextId = userChecker.id + 1;
            }
        }
        fclose(fp);
    }

    if (userExists)
    {
        printf("\n\n\t\t\t\tUser already exists! Please choose a different username.\n");
        printf("\n\n\t\t\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }

    // Get password with hidden input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return;
    }

    printf("\n\n\t\t\t\tEnter password: ");
    scanf("%s", pass);

    // Restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return;
    }

    // Save new user to file
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(fp, "%d %s %s\n", nextId, a, pass);
    fclose(fp);

    printf("\n\n\t\t\t\tRegistration successful! You can now login.\n");
    printf("\n\n\t\t\t\tPress Enter to continue...");
    getchar();
    getchar();
}