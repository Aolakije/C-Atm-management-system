-- Create users table

-- DROP TABLE IF EXISTS users;

-- CREATE TABLE users (
--     user_id INTEGER PRIMARY KEY AUTOINCREMENT,
--     name TEXT NOT NULL UNIQUE,
--     password TEXT NOT NULL
-- );

-- INSERT INTO users (name, password) VALUES ('Tyc123', '12');
-- INSERT INTO users (name, password) VALUES ('Alice', '123');
-- INSERT INTO users (name, password) VALUES ('Nathan', '1234');
-- -- Create accounts table


-- DROP TABLE IF EXISTS accounts;
-- CREATE TABLE IF NOT EXISTS accounts (
--     id INTEGER PRIMARY KEY AUTOINCREMENT,
--     user_id INTEGER NOT NULL,
--     username TEXT NOT NULL,
--     account_id INTEGER NOT NULL UNIQUE,
--     creation_date TEXT NOT NULL,
--     country TEXT NOT NULL,
--     phone_number TEXT NOT NULL,
--     balance REAL NOT NULL DEFAULT 0.0,
--     account_type TEXT NOT NULL,
--     FOREIGN KEY (user_id) REFERENCES users(user_id)
-- );

SELECT * FROM users;

SELECT * FROM accounts;