#include "Database.h"
#include <iostream>
#include <sqlite3.h>

Database::Database(const std::string& dbName) : databaseName(dbName), db(nullptr) {
    if (sqlite3_open(databaseName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        InitializeDatabase();
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

void Database::InitializeDatabase() {
    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS clients (
            id TEXT PRIMARY KEY,
            score INTEGER,
            key_up INTEGER,
            key_down INTEGER,
            key_left INTEGER,
            key_right INTEGER,
            key_shoot INTEGER
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

bool Database::AddClient(const std::string& clientId, int score) {
    const char* insertSQL = "INSERT INTO clients (id, score) VALUES (?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, clientId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

bool Database::ClientExists(const std::string& clientId) {
    const char* existsSQL = "SELECT 1 FROM clients WHERE id = ? LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, existsSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, clientId.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);

    return exists;
}

bool Database::DeleteClient(const std::string& clientId) {
    const char* deleteSQL = "DELETE FROM clients WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, deleteSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, clientId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

bool Database::UpdateScore(const std::string& clientId, int newScore) {
    const char* updateSQL = "UPDATE clients SET score = ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, newScore);
    sqlite3_bind_text(stmt, 2, clientId.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

int Database::GetScore(const std::string& clientId) {
    const char* selectSQL = "SELECT score FROM clients WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, clientId.c_str(), -1, SQLITE_TRANSIENT);

    int score = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        score = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return score;
}

bool Database::SaveKeyBindings(const std::string& clientId, int up, int down, int left, int right, int shoot) {
    const char* updateSQL = R"(
        UPDATE clients 
        SET key_up = ?, key_down = ?, key_left = ?, key_right = ?, key_shoot = ? 
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, up);
    sqlite3_bind_int(stmt, 2, down);
    sqlite3_bind_int(stmt, 3, left);
    sqlite3_bind_int(stmt, 4, right);
    sqlite3_bind_int(stmt, 5, shoot);
    sqlite3_bind_text(stmt, 6, clientId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    if (success) {
        std::cout << "Key bindings for client '" << clientId << "' saved successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to save key bindings for client '" << clientId << "'. Error: " << sqlite3_errmsg(db) << std::endl;
    }

    return success;
}

bool Database::GetKeyBindings(const std::string& username, int& up, int& down, int& left, int& right, int& shoot) {
    const char* selectSQL = R"(
        SELECT key_up, key_down, key_left, key_right, key_shoot 
        FROM clients WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        up = sqlite3_column_int(stmt, 0);
        down = sqlite3_column_int(stmt, 1);
        left = sqlite3_column_int(stmt, 2);
        right = sqlite3_column_int(stmt, 3);
        shoot = sqlite3_column_int(stmt, 4);
    }
    else {
        std::cerr << "No key bindings found for username: " << username << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
