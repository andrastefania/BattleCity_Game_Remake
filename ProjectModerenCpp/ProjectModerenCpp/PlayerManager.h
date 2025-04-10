#pragma once

#include <vector>
#include <memory>
#include "Player.h"

class PlayerManager {
public:
    PlayerManager();

    void AddPlayer(const std::shared_ptr<Player>& player);

    void RemovePlayer(const std::string& name);

    //Getters
    std::shared_ptr<Player> GetPlayer(const std::string& name) const;
    const std::vector<std::shared_ptr<Player>>& GetAllPlayers() const;
    size_t GetPlayerCount() const;

    void DisplayHighScores(int score) const;
private:
    std::vector<std::shared_ptr<Player>> m_players; 
};
