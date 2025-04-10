#pragma once
#define _ITERATOR_DEBUG_LEVEL 2

#include "ClientServer.h"
#include <stdexcept>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>
#include <iostream>


#include "libs/nlohmann/json.hpp"
using json = nlohmann::json;


constexpr auto SERVER_URL = "http://localhost:8080";
std::string ClientServer::m_username = "";
std::string ClientServer::m_gameCode = "";

void ClientServer::connectServer()
{
    qDebug() << "Am intrat in functia de conectare la server din ClientServer.";
    try
    {
        cpr::Response response = cpr::Get(cpr::Url{ std::string(SERVER_URL) + "/connect" });

        if (response.status_code == 200)
        {
            std::cout << "Mesaj de la server: " << response.text << std::endl;
        }
        else
        {
            std::cerr << "Eroare la conectare. Cod răspuns: " << response.status_code << std::endl;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Excepție la conectare: " << ex.what() << std::endl;
    }
}

void ClientServer::StartGameWindow()
{
    qDebug() << "Am intrat in functia StartGameWindow din ClientServer.";
    try {
        cpr::Response response = cpr::Get(cpr::Url{ std::string(SERVER_URL) + "/startGame" });

        if (response.status_code == 200) {
            std::cout << "Mesaj de la server: " << response.text << std::endl;
        }
        else {
            std::cerr << "Eroare la conectare. Cod răspuns: " << response.status_code << " - " << response.text << std::endl;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Excepție la conectare: " << ex.what() << std::endl;
    }
}

bool ClientServer::LoginClient(const std::string& username) {
    try {
        m_username = username;
        auto response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/login" },
            cpr::Body{ "{\"username\":\"" + username + "\"}" },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Login successful: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Login failed: " << response.text << " (Code: " << response.status_code << ")" << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception during login: " << ex.what() << std::endl;
        return false;
    }
}

bool ClientServer::RegisterClient(const std::string& username) {
    try {

        m_username = username;
        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/register" },
            cpr::Body{ "{\"username\":\"" + username + "\"}" },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            qDebug() << "Registration successful: " << QString::fromStdString(response.text);
            return true;
        }
        else {
            qDebug() << "Registration failed. Status code: " << response.status_code;
            return false;
        }
    }
    catch (const std::exception& ex) {
        qDebug() << "Exception during registration: " << ex.what();
        return false;
    }
}

void ClientServer::FetchAndProcessMap(const std::string& gameCode) {
    try {
        cpr::Response response = cpr::Get(
              cpr::Url{ std::string(SERVER_URL) + "/get_map" },
              cpr::Parameters{ {"game_code", gameCode} } 
     );

        if (response.status_code == 200) {
            json mapData = json::parse(response.text);
            QJsonArray qMapArray;

            for (const auto& row : mapData["map"]) {
                QJsonArray qRow;
                for (const auto& cell : row) {
                    qRow.append(static_cast<int>(cell));
                }
                qMapArray.append(qRow);
            }
            this->mapData = qMapArray;
        }
        else {
            qDebug() << "Eroare la fetch map: Cod status =" << response.status_code;
        }
    }
    catch (const std::exception& ex) {
        qDebug() << "Excepție: " << ex.what();
    }
}

QJsonArray ClientServer::GetMap()
{
    return mapData;
}

bool ClientServer::ControlsClient(const std::string& controls)
{
    try {
        nlohmann::json jsonBody;
        jsonBody["username"] = ClientServer::m_username;  
        jsonBody["controls"] = nlohmann::json::parse(controls);

        std::string jsonString = jsonBody.dump();

        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/controls" },
            cpr::Body{ jsonString }, 
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Success: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Error: " << response.status_code << " " << response.text << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception during controls submission: " << ex.what() << std::endl;
        return false;
    }
    return false;
}


std::string ClientServer::GenerateCode(uint8_t level, const std::string& username)
{
    try {
        std::string name = ClientServer::m_username;
        cpr::Response response = cpr::Get(
            cpr::Url{ std::string(SERVER_URL) + "/generate_code" },
            cpr::Parameters{
                {"level", std::to_string(level)},
                {"username", name}            
            },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Generated Game Code: " << response.text << std::endl;
            m_gameCode = response.text;
            return response.text; 
        }
        else {
            std::cerr << "Error: " << response.status_code << " " << response.text << std::endl;
            return "";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception during generating game code: " << ex.what() << std::endl;
        return "";
    }
}

bool ClientServer::JoinGame(const std::string& gameCode, const std::string& username)
{
    try {
        nlohmann::json jsonBody;
        jsonBody["game_code"] = gameCode;
        std::string name = ClientServer::m_username;
        jsonBody["username"] = name;
        m_gameCode = gameCode;
        std::string jsonString = jsonBody.dump();

        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/join_game" },
            cpr::Body{ jsonString },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Successfully joined the game: " << response.text << std::endl;
            return true;
        }
        else 
            if (response.status_code == 410)
            {
                std::cout << "There are already 4 players in this game" << response.text << std::endl;
                return true;
            }
        else {
            std::cerr << "Failed to join the game. Server response: " << response.status_code << " " << response.text << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception during joining the game: " << ex.what() << std::endl;
        return false;
    }
}

bool ClientServer::SendKeyPress(int keyCode) {
    try {
        std::string name = ClientServer::m_username;
        std::string code = ClientServer::m_gameCode;

        nlohmann::json keyPressJson;
        keyPressJson["username"] = name;
        keyPressJson["code"] = code;
        keyPressJson["key_code"] = keyCode;

        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/key_press" },
            cpr::Body{ keyPressJson.dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Key press sent successfully: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to send key press. Response: " << response.text << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception while sending key press: " << ex.what() << std::endl;
        return false;
    }
}

bool ClientServer::IsLastPlayer()
{
    std::string name = ClientServer::m_username;
    std::string code = ClientServer::m_gameCode;

    json requestBody = {
        {"username", name},
        {"game_code", code}
    };

    cpr::Response response = cpr::Post(
        cpr::Url{ std::string(SERVER_URL) + "/is_last_player" },
        cpr::Body{ requestBody.dump() },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << response.text << std::endl;
        return true;
    }
    else {
        std::cerr << "Error: " << response.status_code << " - " << response.text << std::endl;
        return false;
    }
}



bool ClientServer::SaveSettings(const std::string& volume)
{
    try {
        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/save_general_settings" },
            cpr::Body{ "{\"volume\":\"" + volume + "\"}" },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "Settings saved successfully: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to save settings: " << response.text << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error during settings save request: " << ex.what() << std::endl;
        return false;
    }
}

void ClientServer::GetGeneralSettings() {
    try {
        cpr::Response response = cpr::Get(cpr::Url{ std::string(SERVER_URL) + "/get_general_settings" });

        if (response.status_code == 200) {
            std::cout << "General settings retrieved: " << response.text << std::endl;

        }
        else {
            std::cerr << "Failed to retrieve general settings. Response code: " << response.status_code << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception while getting general settings: " << ex.what() << std::endl;
    }
}
bool ClientServer::SetGeneralSettings(const std::string& settingsJson) {
    try {
        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/set_general_settings" },
            cpr::Body{ settingsJson },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "General settings updated: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to update general settings. Response code: " << response.status_code << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception while setting general settings: " << ex.what() << std::endl;
        return false;
    }
}
void ClientServer::GetInGameSettings() {
    try {
        cpr::Response response = cpr::Get(cpr::Url{ std::string(SERVER_URL) + "/get_in_game_settings" });

        if (response.status_code == 200) {
            std::cout << "In game settings retrieved: " << response.text << std::endl;

        }
        else {
            std::cerr << "Failed to retrieve in game settings. Response code: " << response.status_code << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception while getting in game settings: " << ex.what() << std::endl;
    }
}
bool ClientServer::SetInGameSettings(const std::string& settingsJson) {
    try {
        cpr::Response response = cpr::Post(
            cpr::Url{ std::string(SERVER_URL) + "/set_in_game_settings" },
            cpr::Body{ settingsJson },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code == 200) {
            std::cout << "In game settings updated: " << response.text << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to update in game settings. Response code: " << response.status_code << std::endl;
            return false;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception while setting in game settings: " << ex.what() << std::endl;
        return false;
    }
}

void ClientServer::RefreshGameMapIncrementally() {
    std::string code = ClientServer::m_gameCode;
    cpr::Response response = cpr::Get(cpr::Url{ std::string(SERVER_URL) + "/get_map_changes" },
        cpr::Parameters{ {"game_code", code} });

    if (response.status_code == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject changes = doc.object();

        QJsonArray changedCells = changes["changed_cells"].toArray();

        for (const QJsonValue& cell : changedCells) {
            QJsonObject cellData = cell.toObject();
            int xNew = cellData["new_x"].toInt();
            int xLast = cellData["last_x"].toInt();
            int yNew = cellData["new_y"].toInt();
            int yLast = cellData["last_y"].toInt();
            QString type = cellData["type"].toString();

            if(type=="player")
                UpdateMapCellPlayer(xNew, yNew, xLast, yLast);
            if (type == "wall")
                UpdateMapCellWall(xNew, yNew, xLast, yLast);
        }
    }
    else {
        qDebug() << "Failed to fetch map changes from the server.";
    }
}
void ClientServer::UpdateMapCellPlayer(int startX, int startY, int stopX, int stopY) {
    ClientServer client;
    if (startX < 0 || startY < 0 || stopX < 0 || stopY < 0 ||
        startY >= client.mapData.size() || stopY >= client.mapData.size() ||
        startX >= client.mapData[startY].toArray().size() || stopX >= client.mapData[stopY].toArray().size()) {
        qDebug() << "Invalid coordinates for update.";
        return;
    }
    QJsonArray startRow = client.mapData[startY].toArray();
    QJsonArray stopRow = client.mapData[stopY].toArray();

    int startValue = startRow[startX].toInt();
    int stopValue = stopRow[stopX].toInt();

    startRow[startX] = stopValue;
    stopRow[stopX] = startValue;

    client.mapData[startY] = startRow;
    client. mapData[stopY] = stopRow;
    emit client.mapWidget->setMapData(client.mapData);
}

void ClientServer::UpdateMapCellWall(int startX, int startY, int stopX, int stopY) {
    ClientServer client;
    if (startX < 0 || startY < 0 || stopX < 0 || stopY < 0 ||
        startY >= client.mapData.size() || stopY >= client.mapData.size() ||
        startX >= client.mapData[startY].toArray().size() || stopX >= client.mapData[stopY].toArray().size()) {
        qDebug() << "Invalid coordinates for update.";
        return;
    }
    QJsonArray startRow = client.mapData[startY].toArray();
    QJsonArray stopRow = client.mapData[stopY].toArray();

    int startValue = startRow[startX].toInt();
    int stopValue = stopRow[stopX].toInt();

    startRow[startX] = stopValue;
    stopRow[stopX] = startValue;

    client.mapData[startY] = startRow;
    client.mapData[stopY] = stopRow;
    emit client.mapWidget->setMapData(client.mapData);

}