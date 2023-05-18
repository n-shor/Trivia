#pragma once

#include "sqlite3.h"
#include "IDatabase.h"
#include <iostream>
#include <string>
#include <stdexcept>

class SqliteDatabase : public IDatabase {
private:
    sqlite3* db;

public:
    SqliteDatabase() : db(nullptr) {}

    bool open() override {
        if (sqlite3_open("Trivia.sqlite", &db) != SQLITE_OK) {
            std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* sqlUsers = "CREATE TABLE IF NOT EXISTS USERS("
            "USERNAME TEXT NOT NULL,"
            "PASSWORD TEXT NOT NULL,"
            "EMAIL TEXT NOT NULL);";

        const char* sqlTrivia = "CREATE TABLE IF NOT EXISTS TriviaQuestions("
            "ID INT PRIMARY KEY,"
            "Question TEXT NOT NULL,"
            "OptionA TEXT NOT NULL,"
            "OptionB TEXT NOT NULL,"
            "OptionC TEXT NOT NULL,"
            "OptionD TEXT NOT NULL,"
            "CorrectAnswer CHAR NOT NULL);";

        if (sqlite3_exec(db, sqlUsers, nullptr, 0, nullptr) != SQLITE_OK ||
            sqlite3_exec(db, sqlTrivia, nullptr, 0, nullptr) != SQLITE_OK) {
            std::cout << "Error creating tables: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        std::cout << "SQLite database connection opened and tables created if they didn't exist." << std::endl;
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
            throw std::runtime_error("Error preparing SQL statement");
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
            throw std::runtime_error("Error preparing SQL statement");
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

    bool addQuestion(int id, std::string question, std::string optionA, std::string optionB, std::string optionC, std::string optionD, char correctAnswer) {
        std::string sql = "INSERT INTO TriviaQuestions (ID, Question, OptionA, OptionB, OptionC, OptionD, CorrectAnswer) "
            "VALUES (" + std::to_string(id) + ", '" + question + "', '" + optionA + "', '" + optionB + "', '" + optionC + "', '" + optionD + "', '" + correctAnswer + "');";

        if (sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr) != SQLITE_OK) {
            std::cout << "Error inserting question" << std::endl;
            return false;
        }

        std::cout << "Question added successfully to the database!" << std::endl;
        return true;
    }

    Question getQuestion(int id) {
        std::string sql = "SELECT * FROM TriviaQuestions WHERE ID = " + std::to_string(id) + ";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        Question question;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            question.id = sqlite3_column_int(stmt, 0);
            question.question = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            question.optionA = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            question.optionB = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            question.optionC = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            question.optionD = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            question.correctAnswer = *reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        }

        sqlite3_finalize(stmt);
        return question;
    }

    char getCorrectAnswer(int id) {
        std::string sql = "SELECT CorrectAnswer FROM TriviaQuestions WHERE ID = " + std::to_string(id) + ";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Error preparing SQL statement");
        }

        char correctAnswer = '\0';
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            correctAnswer = *reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }

        sqlite3_finalize(stmt);
        return correctAnswer;
    }
};
