-- Create users table
CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    password TEXT NOT NULL
);

-- Create accounts table
CREATE TABLE IF NOT EXISTS accounts (
    id INTEGER PRIMARY KEY,
    user_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    account_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    country TEXT NOT NULL,
    phone TEXT NOT NULL,
    balance REAL NOT NULL,
    account_type TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);