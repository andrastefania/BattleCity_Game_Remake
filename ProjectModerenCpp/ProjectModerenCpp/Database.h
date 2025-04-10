#pragma once
#include <sqlite3.h>
#include <string>

class Database
{
public:
    Database(const std::string& dbName);
    ~Database();

    bool AddClient(const std::string& clientId, int score);
    bool ClientExists(const std::string& clientId);
    bool DeleteClient(const std::string& clientId);

    bool UpdateScore(const std::string& clientId, int newScore);
    int GetScore(const std::string& clientId);

    bool SaveKeyBindings(const std::string& clientId, int up, int down, int left, int right, int shoot);
    bool GetKeyBindings(const std::string& username, int& up, int& down, int& left, int& right, int& shoot);

private:
    std::string databaseName;
    void InitializeDatabase();
    sqlite3* db;
};
