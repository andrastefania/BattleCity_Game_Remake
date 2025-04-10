#pragma once
#include "routing.h"
#include <sstream>
#include <regex>
#include "GameManager.h"

#include "libs/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace http;

Database db("NovaDatabase5.db");
namespace http {
    std::unordered_map<std::string, std::shared_ptr<Player>> playersActive;
    std::unordered_map<std::string, Game> games;
}


void http::Routing::Run()
{
    CROW_ROUTE(m_app, "/connect")([]() {
        return crow::response(200, "Server: Conectare reusita!");
        });
    CROW_ROUTE(m_app, "/startGame")([]() {
        return crow::response(200, "Server: Conectare reusita la start!");
        });

    CROW_ROUTE(m_app, "/login").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("username")) {
            return crow::response(400, "Invalid JSON");
        }

        std::string username = json["username"].s();
        if (db.ClientExists(username)) {

            if (playersActive.find(username) != playersActive.end()) {
                return crow::response(403, "Player is already active");
            }
            std::shared_ptr<Player> player = std::make_shared<Player>(username, db);
            playersActive.emplace(username, player);

            return crow::response(200, "Login successful");
        }
        else {
            return crow::response(401, "Invalid username");
        }
    });

    CROW_ROUTE(m_app, "/register").methods(crow::HTTPMethod::POST)([](const crow::request& req) {

        auto json = crow::json::load(req.body);
        if (!json || !json.has("username")) {
            return crow::response(400, "Invalid JSON or missing 'username' field");
        }
        std::string username = json["username"].s();
        std::cout << username;
        try {
            std::regex usernameRegex("^(?=.*[A-Z])[A-Za-z0-9]{3,}$");
            if (!std::regex_match(username, usernameRegex)) {
                return crow::response(401, "Invalid username: The name must contain at least one uppercase letter and no special characters.");
            }
        }
        catch (const std::regex_error& e) {
            return crow::response(500, "Internal server error: Invalid regex configuration.");
        }
        if (db.ClientExists(username)) {
            return crow::response(409, "Registration failed: User already exists");
        }
        db.AddClient(username, 0);
        std::shared_ptr<Player> player = std::make_shared<Player>(username, db);
        playersActive.emplace(username, player);

        return crow::response(200, "Registration successful");
        });


    CROW_ROUTE(m_app, "/controls").methods("POST"_method)([](const crow::request& req) {
        nlohmann::json json;
        try {
            json = nlohmann::json::parse(req.body);
        }
        catch (const std::exception& e) {
            return crow::response(400, "Invalid JSON format: " + std::string(e.what()));
        }
        if (!json.contains("username") ||
            !json.contains("controls") || 
            !json["controls"].contains("Up") || !json["controls"].contains("Down") ||
            !json["controls"].contains("Left") || !json["controls"].contains("Right") ||
            !json["controls"].contains("Shoot")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }
        std::string username = json["username"];
        auto controls = json["controls"];
        int up = controls["Up"];
        int down = controls["Down"];
        int left = controls["Left"];
        int right = controls["Right"];
        int shoot = controls["Shoot"];

        if (playersActive.find(username) == playersActive.end()) {
            return crow::response(404, "User is not Active");
        }
        bool success = db.SaveKeyBindings(username, up, down, left, right, shoot);
        if (!success) {
            return crow::response(500, "Failed to save key bindings");
        }

        return crow::response(200, "Controls successfully set for user: " + username);
    });

    CROW_ROUTE(m_app, "/generate_code").methods("GET"_method)([](const crow::request& req) {
        try {

            auto levelStr = req.url_params.get("level");
            auto username = req.url_params.get("username");

            if (levelStr == nullptr || username == nullptr) {
                return crow::response(400, "Invalid parameters");
            }

            uint8_t level = static_cast<uint8_t>(std::stoi(levelStr));
            std::string gameCode = Game::GenerateGameCode();

            while (games.find(gameCode) != games.end()) {
                gameCode = Game::GenerateGameCode();
            }
            Game game(level, gameCode);
            if (playersActive.find(username) == playersActive.end()) {
                return crow::response(404, "Player not found or not active");
            }
            std::shared_ptr<Player> player = playersActive[username];
            game.AddPlayer(player);
            games.emplace(gameCode, game);
            return crow::response(200,  gameCode);
        }
        catch (const std::exception& e) {
            return crow::response(500, "Error generating game code: " + std::string(e.what()));
        }
        });


    CROW_ROUTE(m_app, "/join_game").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("game_code") || !json.has("username")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }
        std::string gameCode = json["game_code"].s();
        std::string username = json["username"].s();
    
        if (games.find(gameCode) == games.end()) {
            return crow::response(404, "Game code not found");
        }
    
        Game& game = games[gameCode];
        if (game.IsGameStarted()) {
            return crow::response(403, "Game already started");
        }
        if (game.IsMaxPlayersReached()) {
            return crow::response(410, "Maximum player limit reached, cannot join the game");
        }
        if (game.IsPlayerInGame(username)) {
            return crow::response(409, "Player already in the game");
        }
        std::shared_ptr<Player> player = playersActive[username];
        game.AddPlayer(player);

        return crow::response(200, "Player " + username + " joined the game successfully");
        });


    CROW_ROUTE(m_app, "/get_map").methods("GET"_method)([](const crow::request& req) {
        try {

            std::string gameCode = req.url_params.get("game_code"); 

            if (games.find(gameCode) == games.end()) {
                return crow::response(404, "Game code not found");
            }

            Game& game = games[gameCode];  
            Map gameMap= game.GetMap();
            //gameMap.DisplayMap(); 

            crow::json::wvalue json;
            json["width"] = gameMap.GetWidth();
            json["height"] = gameMap.GetHeight();

            const auto& mapData = gameMap.GetMap();
            crow::json::wvalue::list jsonMap;
            for (const auto& row : mapData) {
                crow::json::wvalue::list jsonRow;
                for (const auto& cell : row) {
                    jsonRow.push_back(cell.ToInt());
                }
                jsonMap.push_back(std::move(jsonRow));
            }
            json["map"] = std::move(jsonMap);

            std::string serializedJson = json.dump();
           // std::cout << "Serialized JSON Response: " << serializedJson << std::endl;

            return crow::response(200, serializedJson);
        }
        catch (const std::exception& e) {
            return crow::response(500, "Server error: " + std::string(e.what()));
        }
        });


    CROW_ROUTE(m_app, "/key_press").methods("POST"_method)([](const crow::request& req) {
        try {
            auto json = crow::json::load(req.body);
            if (!json.has("username") || !json.has("key_code") || !json.has("code")) {
                return crow::response(400, "Missing 'username' or 'key_code' field");
            }
            std::string username = json["username"].s();
            std::string code = json["code"].s();
            int keyCode = json["key_code"].i();

            if (playersActive.find(username) == playersActive.end()) {
                return crow::response(404, "Player not found or not active");
            }

            if (games.find(code) == games.end()) {
                return crow::response(404, "Game not found");
            }
            int up, down, left, right, shoot;
            if (!db.GetKeyBindings(username, up, down, left, right, shoot)) {
                return crow::response(500, "Failed to fetch key bindings from the database");
            }
            
            std::shared_ptr<Player> player = playersActive[username];
            Game& game = games[code];
            MovementObject::Direction direction;
            if (keyCode == up) {
                direction = MovementObject::Direction::Up;
            }
            else if (keyCode == down) {
                direction = MovementObject::Direction::Down;
            }
            else if (keyCode == left) {
                direction = MovementObject::Direction::Left;
            }
            else if (keyCode == right) {
                direction = MovementObject::Direction::Right;
            }
            else if (keyCode == shoot) {

                game.ShootBulletS(player);
                return crow::response(200, "Shoot action processed");
                std::cout << "Player " << username << " shot a bullet." << std::endl;
                return crow::response(200, "Shoot action processed");
            }
            else {
                return crow::response(400, "Invalid key code");
            }

            game.MovePlayer(username, direction);
            //std::cout << "Player " << username << " moved in direction " << static_cast<int>(direction) << std::endl;
            /*Map gameMap = game.GetMap();
            gameMap.DisplayMap();*/

            return crow::response(200, "Key press processed successfully");
        }
        catch (const std::exception& ex) {
            return crow::response(500, "Server error: " + std::string(ex.what()));
        }
        });

    CROW_ROUTE(m_app, "/get_map_changes").methods("GET"_method)([](const crow::request& req) {
        try {
            std::string gameCode = req.url_params.get("game_code");

            if (gameCode.empty()) {
                return crow::response(400, "Game code is required");
            }
            Game& game = games[gameCode];
            const auto& changedCells = game.GetChangedCells();

            nlohmann::json responseJson;
            for (const auto& change : changedCells) {
                const auto& [newCoord, lastCoord, type] = change;
                nlohmann::json changedCell;
                changedCell["new_x"] = newCoord.first;
                changedCell["new_y"] = newCoord.second;
                changedCell["last_x"] = lastCoord.first;
                changedCell["last_y"] = lastCoord.second;
                changedCell["type"] = type;

                responseJson["changed_cells"].push_back(changedCell);
            }
            crow::response res;
            res.code = 200;
            res.write(responseJson.dump());
            /*std::string serializedJson = responseJson.dump();
            std::cout << "Serialized JSON Response: " << serializedJson << std::endl;*/
            game.ClearChangedCells();
            return res;
        }
        catch (const std::exception& e) {
            return crow::response(500, "Error generating map changes: " + std::string(e.what()));
        }
        });

    CROW_ROUTE(m_app, "/is_last_player").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("username") || !json.has("game_code")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }

        std::string username = json["username"].s();
        std::string gameCode = json["game_code"].s();

        if (games.find(gameCode) == games.end()) {
            return crow::response(404, "Game code not found");
        }
        Game& game = games[gameCode];

        if (game.IsLastPlayer(username)) {
            game.RemovePlayer(username);
            return crow::response(200, "Player " + username + " is the last player and has been removed");
        }
        else {
            game.RemovePlayer(username);
            return crow::response(400, "Player " + username + " is not the last player");
        }
    });

    m_app.port(8080).run();
}
