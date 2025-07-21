#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}


void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,           // Fixed: removed & and ->
            u.id,           // Fixed: removed & and ->
            u.name,         // Fixed: removed & and ->
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,      // Assuming you want country here
            r.phone,
            r.amount,
            r.accountType);
}
void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u)
{
    FILE *fp, *tempFp;
    struct Record r;
    int accountId, choice, found = 0;
    char newCountry[100];
    int newPhone;

    system("clear");
    printf("\n\n\t\t======= Update Account Information =======\n\n");
    
    printf("\n\t\tEnter account ID to update: ");
    scanf("%d", &accountId);

    // Check if account exists and belongs to user
    fp = fopen("./data/records.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening records file!\n");
        return;
    }

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s", 
                  &r.id, &r.userId, r.name, &r.phone, 
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.accountNbr, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("\n\t\tAccount not found or doesn't belong to you!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    } 

    printf("\n\t\tWhat would you like to update?\n");
    printf("\n\t\t[1] Phone number\n");
    printf("\n\t\t[2] Country\n");
    printf("\n\t\tChoice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("\n\t\tEnter new phone number: ");
        scanf("%d", &newPhone);
        break;
    case 2:
        printf("\n\t\tEnter new country: ");
        scanf("%s", newCountry);
        break;
    default:
        printf("\n\t\tInvalid choice!\n");
        return;
    }

    // Update the file
    fp = fopen("./data/records.txt", "r");
    tempFp = fopen("./data/temp.txt", "w");
    
    if (fp == NULL || tempFp == NULL)
    {
        printf("Error opening files!\n");
        return;
    }

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s", 
                  &r.id, &r.userId, r.name, &r.phone, 
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.accountNbr, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            // Update the record
            if (choice == 1)
                r.phone = newPhone;
            else if (choice == 2)
                strcpy(r.country, newCountry);
        }
        
        fprintf(tempFp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", 
                r.id, r.userId, r.name, r.phone, 
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.accountNbr, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(tempFp);

    // Replace original file with updated file
    remove("./data/records.txt");
    rename("./data/temp.txt", "./data/records.txt");

    printf("\n\t\tAccount information updated successfully!\n");
    printf("\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}
void checkAccountDetails(struct User u)
{
    FILE *fp;
    struct Record r;
    int accountId, found = 0;
    
    system("clear");
    printf("\n\n\t\t======= Check Account Details =======\n\n");
    
    printf("\n\t\tEnter account ID to check: ");
    scanf("%d", &accountId);
    
    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening records file!\n");
        return;
    }
    
    // Read using the format that matches your getAccountFromFile function
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            found = 1;
            printf("\n\t\t====== Account Details ======\n");
            printf("\n\t\tAccount ID: %d", r.id);
            printf("\n\t\tAccount Number: %d", r.accountNbr);
            printf("\n\t\tDeposit Date: %d/%d/%d", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("\n\t\tCountry: %s", r.country);
            printf("\n\t\tPhone: %d", r.phone);
            printf("\n\t\tAmount: $%.2f", r.amount);
            printf("\n\t\tAccount Type: %s", r.accountType);
            
            // Calculate and display interest
            if (strcmp(r.accountType, "savings") == 0)
            {
                double monthlyInterest = (r.amount * 0.07) / 12;
                printf("\n\t\tInterest: You will get $%.2f as interest on day %d of every month", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                double monthlyInterest = (r.amount * 0.04) / 12;
                printf("\n\t\tInterest: You will get $%.2f as interest on day %d of every month", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                double monthlyInterest = (r.amount * 0.05) / 12;
                printf("\n\t\tInterest: You will get $%.2f as interest on day %d of every month", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                double monthlyInterest = (r.amount * 0.08) / 12;
                printf("\n\t\tInterest: You will get $%.2f as interest on day %d of every month", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\n\t\tInterest: You will not get interests because the account is of type current");
            }
            
            printf("\n\t\t===============================\n");
            break;
        }
    }
    
    fclose(fp);
    
    if (!found)
    {
        printf("\n\t\tAccount not found or doesn't belong to you!\n");
    }
    
    printf("\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}

void makeTransaction(struct User u)
{
    FILE *fp, *tempFp;
    struct Record r;
    int accountId, transactionType, found = 0;
    double amount;
    
    system("clear");
    printf("\n\n\t\t======= Make Transaction =======\n\n");
    
    printf("\n\t\tEnter account ID for transaction: ");
    scanf("%d", &accountId);
    
    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening records file!\n");
        return;
    }
    
    // First, check if account exists and get account details
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    
    if (!found)
    {
        printf("\n\t\tAccount not found or doesn't belong to you!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    // Check if account type allows transactions
    if (strcmp(r.accountType, "fixed01") == 0 || 
        strcmp(r.accountType, "fixed02") == 0 || 
        strcmp(r.accountType, "fixed03") == 0)
    {
        printf("\n\t\tError: Transactions are not allowed for fixed accounts!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t\tCurrent balance: $%.2f", r.amount);
    printf("\n\t\tTransaction type:\n");
    printf("\n\t\t[1] Deposit\n");
    printf("\n\t\t[2] Withdraw\n");
    printf("\n\t\tChoice: ");
    scanf("%d", &transactionType);
    
    printf("\n\t\tEnter amount: $");
    scanf("%lf", &amount);
    
    if (amount <= 0)
    {
        printf("\n\t\tError: Amount must be positive!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    if (transactionType == 1) // Deposit
    {
        r.amount += amount;
        printf("\n\t\tDeposit successful! New balance: $%.2f", r.amount);
    }
    else if (transactionType == 2) // Withdraw
    {
        if (amount > r.amount)
        {
            printf("\n\t\tError: Insufficient funds! Current balance: $%.2f", r.amount);
            printf("\n\t\tPress Enter to continue...");
            getchar();
            getchar();
            return;
        }
        r.amount -= amount;
        printf("\n\t\tWithdrawal successful! New balance: $%.2f", r.amount);
    }
    else
    {
        printf("\n\t\tError: Invalid transaction type!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    // Update the file
    fp = fopen(RECORDS, "r");
    tempFp = fopen("./data/temp.txt", "w");
    
    if (fp == NULL || tempFp == NULL)
    {
        printf("Error opening files!\n");
        return;
    }
    
    struct Record tempR;
    
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempR.id, &tempR.userId, tempR.name, &tempR.accountNbr,
                  &tempR.deposit.month, &tempR.deposit.day, &tempR.deposit.year,
                  tempR.country, &tempR.phone, &tempR.amount, tempR.accountType) != EOF)
    {
        if (tempR.id == accountId && tempR.userId == u.id)
        {
            // Write updated record
            tempR.amount = r.amount;
        }
        
        fprintf(tempFp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                tempR.id, tempR.userId, tempR.name, tempR.accountNbr,
                tempR.deposit.month, tempR.deposit.day, tempR.deposit.year,
                tempR.country, tempR.phone, tempR.amount, tempR.accountType);
    }
    
    fclose(fp);
    fclose(tempFp);
    
    // Replace original file with updated file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    
    printf("\n\t\tTransaction completed successfully!\n");
    printf("\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}

void removeAccount(struct User u)
{
    FILE *fp, *tempFp;
    struct Record r;
    int accountId, found = 0, confirm;
    
    system("clear");
    printf("\n\n\t\t======= Remove Account =======\n\n");
    
    printf("\n\t\tEnter account ID to remove: ");
    scanf("%d", &accountId);
    
    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening records file!\n");
        return;
    }
    
    // First, check if account exists and show details
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            found = 1;
            printf("\n\t\tAccount found:");
            printf("\n\t\tAccount ID: %d", r.id);
            printf("\n\t\tAccount Number: %d", r.accountNbr);
            printf("\n\t\tAccount Type: %s", r.accountType);
            printf("\n\t\tAmount: $%.2f", r.amount);
            break;
        }
    }
    fclose(fp);
    
    if (!found)
    {
        printf("\n\t\tAccount not found or doesn't belong to you!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t\tAre you sure you want to remove this account?");
    printf("\n\t\t[1] Yes\n");
    printf("\n\t\t[0] No\n");
    printf("\n\t\tChoice: ");
    scanf("%d", &confirm);
    
    if (confirm != 1)
    {
        printf("\n\t\tAccount removal cancelled.\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    // Remove the account from file
    fp = fopen(RECORDS, "r");
    tempFp = fopen("./data/temp.txt", "w");
    
    if (fp == NULL || tempFp == NULL)
    {
        printf("Error opening files!\n");
        return;
    }
    
    struct Record tempR;
    
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempR.id, &tempR.userId, tempR.name, &tempR.accountNbr,
                  &tempR.deposit.month, &tempR.deposit.day, &tempR.deposit.year,
                  tempR.country, &tempR.phone, &tempR.amount, tempR.accountType) != EOF)
    {
        // Write all records except the one to be removed
        if (!(tempR.id == accountId && tempR.userId == u.id))
        {
            fprintf(tempFp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                    tempR.id, tempR.userId, tempR.name, tempR.accountNbr,
                    tempR.deposit.month, tempR.deposit.day, tempR.deposit.year,
                    tempR.country, tempR.phone, tempR.amount, tempR.accountType);
        }
    }
    
    fclose(fp);
    fclose(tempFp);
    
    // Replace original file with updated file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    printf("\n\t\tAccount removed successfully!\n");
    printf("\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}
void transferOwnership(struct User u)
{
    FILE *fp, *tempFp, *userFp;
    struct Record r;
    struct User newOwner;
    int accountId, newOwnerId, found = 0, ownerExists = 0;
    
    system("clear");
    printf("\n\n\t\t======= Transfer Account Ownership =======\n\n");
    
    printf("\n\t\tEnter account ID to transfer: ");
    scanf("%d", &accountId);
    
    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening records file!\n");
        return;
    }
    
    // Check if account exists and belongs to current user
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.id == accountId && r.userId == u.id)
        {
            found = 1;
            printf("\n\t\tAccount found:");
            printf("\n\t\tAccount ID: %d", r.id);
            printf("\n\t\tAccount Number: %d", r.accountNbr);
            printf("\n\t\tAccount Type: %s", r.accountType);
            printf("\n\t\tAmount: $%.2f", r.amount);
            break;
        }
    }
    fclose(fp);
    
    if (!found)
    {
        printf("\n\t\tAccount not found or doesn't belong to you!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t\tEnter new owner's user ID: ");
    scanf("%d", &newOwnerId);
    
    // Check if new owner exists
    userFp = fopen("./data/users.txt", "r");
    if (userFp == NULL)
    {
        printf("Error opening users file!\n");
        return;
    }
    
    while (fscanf(userFp, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF)
    {
        if (newOwner.id == newOwnerId)
        {
            ownerExists = 1;
            break;
        }
    }
    fclose(userFp);
    
    if (!ownerExists)
    {
        printf("\n\t\tError: New owner with ID %d not found!\n", newOwnerId);
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    if (newOwnerId == u.id)
    {
        printf("\n\t\tError: Cannot transfer account to yourself!\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    printf("\n\t\tTransferring account to user: %s (ID: %d)", newOwner.name, newOwnerId);
    printf("\n\t\tConfirm transfer?");
    printf("\n\t\t[1] Yes\n");
    printf("\n\t\t[0] No\n");
    printf("\n\t\tChoice: ");
    
    int confirm;
    scanf("%d", &confirm);
    
    if (confirm != 1)
    {
        printf("\n\t\tTransfer cancelled.\n");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    // Update the account ownership
    fp = fopen(RECORDS, "r");
    tempFp = fopen("./data/temp.txt", "w");
    
    if (fp == NULL || tempFp == NULL)
    {
        printf("Error opening files!\n");
        return;
    }
    
    struct Record tempR;
    
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempR.id, &tempR.userId, tempR.name, &tempR.accountNbr,
                  &tempR.deposit.month, &tempR.deposit.day, &tempR.deposit.year,
                  tempR.country, &tempR.phone, &tempR.amount, tempR.accountType) != EOF)
    {
        if (tempR.id == accountId && tempR.userId == u.id)
        {
            // Transfer ownership - update userId and name
            tempR.userId = newOwnerId;
            strcpy(tempR.name, newOwner.name);
        }
        
        fprintf(tempFp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                tempR.id, tempR.userId, tempR.name, tempR.accountNbr,
                tempR.deposit.month, tempR.deposit.day, tempR.deposit.year,
                tempR.country, tempR.phone, tempR.amount, tempR.accountType);
    }
    
    fclose(fp);
    fclose(tempFp);
    
    // Replace original file with updated file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    
    printf("\n\t\tAccount ownership transferred successfully!\n");
    printf("\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}