#pragma once

#include "sqlite3.h"
#include "IDatabase.h"
#include <iostream>
#include <string>

class SqliteDatabase : public IDatabase {
private:
    sqlite3* db;

public:
    SqliteDatabase() : db(nullptr) {}

    bool open() override {
        if (sqlite3_open("users.sqlite", &db) != SQLITE_OK) {
            std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* sql = "CREATE TABLE IF NOT EXISTS USERS("
            "USERNAME TEXT NOT NULL,"
            "PASSWORD TEXT NOT NULL,"
            "EMAIL TEXT NOT NULL);";

        if (sqlite3_exec(db, sql, nullptr, 0, nullptr) != SQLITE_OK) {
            std::cout << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        std::cout << "SQLite database connection opened." << std::endl;
        return true;
    }

    bool close() override {
        sqlite3_close(db);
        std::cout << "SQLite database connection closed." << std::endl;
        return true;
    }

    bool doesUserExist(std::string username) override {
        std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        bool userExists = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userExists = true;
        }

        sqlite3_finalize(stmt);
        return userExists;
    }

    bool doesPasswordMatch(std::string username, std::string password) override {
        std::string sql = "SELECT PASSWORD FROM USERS WHERE USERNAME = '" + username + "';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        bool passwordMatches = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            passwordMatches = (dbPassword == password);
        }

        sqlite3_finalize(stmt);
        return passwordMatches;
    }

    bool addNewUser(std::string username, std::string password, std::string email) override {
        std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "', '" + email + "');";

        if (sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr) != SQLITE_OK) {
            std::cout << "Error inserting user" << std::endl;
            return false;
        }

        std::cout << "User created successfully in the database!" << std::endl;
        return true;
    }
};
