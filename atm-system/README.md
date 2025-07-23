# ATM Management System

A comprehensive ATM management system built in C that allows users to manage bank accounts with various features including account creation, transactions, and account management.

## 📋 Project Overview

This project demonstrates programming logic and adaptation to C programming language. The system provides a complete ATM experience with user authentication, account management, and transaction processing using an SQL database for data persistence.

## 🚀 Features

### Authentication
- **User Login**: Secure user authentication system
- **User Registration**: Register new users with unique usernames

### Account Management
- **Create New Accounts**: Users can create multiple bank accounts
- **View Account Details**: Check details of existing accounts with interest calculations
- **Update Account Information**: Modify country and phone number details
- **Remove Accounts**: Delete existing accounts
- **List Owned Accounts**: View all accounts belonging to the user
- **Transfer Ownership**: Transfer account ownership to another user

### Transaction System
- **Deposit Money**: Add funds to accounts
- **Withdraw Money**: Remove funds from accounts (with balance validation)

### Account Types & Interest Rates
- **Current Account**: No interest earned
- **Savings Account**: 7% annual interest rate
- **Fixed01 (1 Year)**: 4% annual interest rate (no transactions allowed)
- **Fixed02 (2 Years)**: 5% annual interest rate (no transactions allowed)
- **Fixed03 (3 Years)**: 8% annual interest rate (no transactions allowed)

## 🛠️ Technology Stack

- **Programming Language**: C
- **Database**: SQL Database
- **Build System**: Makefile
- **File System**: Organized source code structure

## 📁 Project Structure

```
.
├── Makefile              # Build configuration
├── data.sql             # Database schema and table definitions
└── src/                  # Source code directory
    ├── main.c           # Main program entry point
    ├── auth.c           # Authentication functions
    ├── system.c         # Core system functionality
    ├── db.c             # Database operations and queries
    ├── db.h             # Database function declarations
    └── header.h         # Main header file with declarations
```

## ⚙️ Installation & Setup

### Prerequisites
- GCC compiler
- Make utility
- SQL database system (SQLite/MySQL/PostgreSQL)

### Building the Project
```bash
# Clone or download the project
# Navigate to project directory

# Compile the project
make

# Run the application
./atm
```

### Database Setup
```bash
# Initialize the database with the provided schema
# Run the SQL script to create tables
sqlite3 atm_database.db < data.sql

# Or if using MySQL/PostgreSQL, run:
# mysql -u username -p database_name < data.sql
# psql -d database_name -f data.sql
```

The `data.sql` file contains the database schema with table definitions for:
- Users table (user credentials and information)
- Accounts table (account details and balances)

## 🎯 Usage

### Getting Started
1. Run the application: `./atm`
2. Choose to login with existing credentials or register a new user
3. Navigate through the menu options to manage accounts and transactions

### Menu Options
1. **Login/Register**: Access your account or create a new user
2. **Create Account**: Add a new bank account (current, savings, or fixed)
3. **View Account Details**: Check account information and interest calculations
4. **Update Account**: Modify country or phone number
5. **Remove Account**: Delete an existing account
6. **List Accounts**: View all your accounts
7. **Make Transaction**: Deposit or withdraw money
8. **Transfer Ownership**: Transfer account to another user

### Interest Calculation Example
For a savings account with $1023.20 created on 10/10/2002:
- Interest Rate: 7% annually
- Monthly Interest: $5.97
- Display: "You will get $5.97 as interest on day 10 of every month"

## 🔒 Security Features

- Password-protected user accounts
- Account ownership validation
- Transaction amount validation
- Restricted operations on fixed-term accounts

## 🚫 Restrictions

- **Fixed Accounts**: Fixed01, Fixed02, and Fixed03 accounts do not allow transactions
- **Unique Usernames**: No duplicate usernames allowed
- **Account Ownership**: Users can only modify their own accounts
- **Withdrawal Limits**: Cannot withdraw more than the available balance

## 🤝 Contributing

This project was developed as a programming exercise to demonstrate:
- C programming proficiency
- Database integration
- File system management
- User interface design
- Financial system logic

## 📝 Notes

- All data is persisted in the SQL database
- Transaction history is maintained for audit purposes
- Interest calculations are based on account creation date
- The system handles various account types with different rules and restrictions

## 🛡️ Error Handling

The system includes comprehensive error handling for:
- Invalid user input
- Database connection issues
- Insufficient funds
- Non-existent accounts
- Unauthorized access attempts

---
## 👤 Author

Developed by: ADEOLA
[Aolakije]

This project is for educational purposes and can be extended into a fully functional banking simulation system.


