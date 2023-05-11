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
        int exit = 0;
        exit = sqlite3_open("users.db", &db);

        std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
            "USERNAME TEXT NOT NULL,"
            "PASSWORD TEXT NOT NULL,"
            "EMAIL TEXT NOT NULL);";

        char* messaggeError;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messaggeError);

        if (exit != SQLITE_OK) {
            std::cout << "Error Create Table" << std::endl;
            sqlite3_free(messaggeError);
            return false;
        }
        else {
            std::cout << "SQLite database connection opened." << std::endl;
            return true;
        }
    }

    bool close() override {
        sqlite3_close(db);
        std::cout << "SQLite database connection closed." << std::endl;
        return true;
    }

    bool doesUserExist(std::string username) override {
        std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
            return false;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return true;
        }
        else {
            sqlite3_finalize(stmt);
            return false;
        }
    }

    bool doesPasswordMatch(std::string username, std::string password) override {
        std::string sql = "SELECT PASSWORD FROM USERS WHERE USERNAME = '" + username + "';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
            return false;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string db_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (db_password == password) {
                return true;
            }
        }

        return false;
    }

    bool addNewUser(std::string username, std::string password, std::string email) override {
        std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "', '" + email + "');";
        char* messageError;

        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);

        if (exit != SQLITE_OK) {
            std::cout << "Error Insert" << std::endl;
            sqlite3_free(messageError);
            return false;
        }
        else {
            std::cout << "Records created Successfully!" << std::endl;
            return true;
        }
    }
};
