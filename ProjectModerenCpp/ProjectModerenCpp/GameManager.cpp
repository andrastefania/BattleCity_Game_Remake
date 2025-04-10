#include "GameManager.h"
#include <unordered_map>  

std::unordered_map<std::string, Game> GameManager::m_games;  

bool GameManager::GameExists(const std::string& gameCode) {
    return m_games.find(gameCode) != m_games.end();
}

void GameManager::AddGame(const std::string& gameCode, const Game& game) {
    m_games[gameCode] = game;
}

Game& GameManager::GetGame(const std::string& gameCode) {
    auto it = m_games.find(gameCode);
    if (it == m_games.end()) {
        throw std::runtime_error("Game not found");
    }
    return it->second; 
}
