#include "PlayerManager.h"
#include <iostream>
#include <ranges>

PlayerManager::PlayerManager() = default;

void PlayerManager::AddPlayer(const std::shared_ptr<Player>& player) {
    m_players.push_back(player);
}

void PlayerManager::RemovePlayer(const std::string& name) {
    m_players.erase(
        std::remove_if(m_players.begin(), m_players.end(),
            [&name](const std::shared_ptr<Player>& player) {
                return player->GetName() == name;
            }),
        m_players.end());
}

// Getters
std::shared_ptr<Player> PlayerManager::GetPlayer(const std::string& name) const {
    for (const auto& player : m_players) {
        if (player->GetName() == name) {
            return player;
        }
    }
    return nullptr;
}
const std::vector<std::shared_ptr<Player>>& PlayerManager::GetAllPlayers() const {
    return m_players;
}
size_t PlayerManager::GetPlayerCount() const {
    return m_players.size();
}

void PlayerManager::DisplayHighScores(int score) const
{
    auto highScorers = m_players
        | std::views::filter([&](const auto& p) {
        return p->GetScore() > score;
            });

    /*std::cout << "Players with score above " << threshold << ":\n";
    for (const auto& player : highScorers) {
        std::cout << player->GetName() << " with score " << player->GetScore() << "\n";
    }*/
}
