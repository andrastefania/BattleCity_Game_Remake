#include "CollisionManager.h"
#include "Game.h"
#include <algorithm>
#include <memory>
#include <map>
#include <ranges>

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

CollisionManager::CollisionManager(std::shared_ptr<Map> map, std::shared_ptr<PlayerManager> playerManager)
    : m_map(map), m_playerManager(playerManager) {}

template <typename T>
    requires std::is_integral_v<T>
bool CollisionManager::IsValidPosition(T x, T y) const {
    if (x < 0 || x >= static_cast<T>(m_map->GetWidth())) {
        return false; 
    }
    if (y < 0 || y >= static_cast<T>(m_map->GetHeight())) {
        return false; 
    }
    const auto& cell = m_map->GetCell(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
    if (std::holds_alternative<Wall::TypeWall>(cell.content)) {
        const auto& wallType = std::get<Wall::TypeWall>(cell.content);
        if (wallType == Wall::TypeWall::indestructible || wallType == Wall::TypeWall::destructible) {
            return false;
        }
    }
    return true;
}

std::optional<Map::Cell> CollisionManager::GetValidCell(uint16_t x, uint16_t y) const {
    if (x >= m_map->GetWidth() || y >= m_map->GetHeight()) {
        return std::nullopt; 
    }
    auto& cell = m_map->GetCell(x, y);
    if (typeid(cell.content) == typeid(Wall)) {
        return std::nullopt;
    }
    return cell; 
}

void CollisionManager::HandleBulletCollisions() {
    auto players = m_playerManager->GetAllPlayers();
    for (auto& player : players) {
        auto& movement = player->GetMovementObject();

        if (!movement.IsBulletActive()) continue;

        auto [x, y] = movement.GetPosition(true);

        if (!IsValidPosition(x, y)) {
            movement.DeactivateBullet();
            continue;
        }
        for (auto& targetPlayer : players) {
            if (player == targetPlayer) continue;

            auto& targetMovement = targetPlayer->GetMovementObject();
            if (targetMovement.IsBulletActive()) {
                auto [targetX, targetY] = targetMovement.GetPosition(true);
                if (x == targetX && y == targetY) {
                    movement.DeactivateBullet();
                    targetMovement.DeactivateBullet();
                }
            }
        }
    }
}

void CollisionManager::HandleTankCollisions() {
    auto players = m_playerManager->GetAllPlayers();

    std::unordered_map<std::pair<uint16_t, uint16_t>, std::shared_ptr<Player>, PairHash> positionMap;

    for (auto& player : players) {
        auto [x, y] = player->GetMovementObject().GetPosition(false);
        positionMap[{x, y}] = player;
    }

    for (auto& player : players) {
        auto& movement = player->GetMovementObject();
        auto [currentX, currentY] = movement.GetPosition(false);
        auto [targetX, targetY] = movement.GetPosition(true);

        if (!IsValidPosition(targetX, targetY) || positionMap.contains({ targetX, targetY })) {
            continue;
        }

        movement.SetPosition(targetX, targetY);
        m_map->SetCellContent(currentX, currentY, Map::Empty{});
        m_map->SetCellContent(targetX, targetY, Map::Tank{});

        positionMap.erase({ currentX, currentY });
        positionMap[{targetX, targetY}] = player;
    }
}

bool CollisionManager::isOccupiedByAnotherTank(const std::vector<std::shared_ptr<Player>>& players, uint16_t x, uint16_t y, const std::shared_ptr<Player>& currentPlayer) {
    auto positionOccupied = std::ranges::any_of(players, [&](const auto& player) {
        if (player == currentPlayer) return false; 
        auto [px, py] = player->GetMovementObject().GetPosition(false);
        return (px == x && py == y); 
        });
    return positionOccupied;
}

void CollisionManager::HandleBulletTankCollisions() {
    auto players = m_playerManager->GetAllPlayers();

    auto deactivateIfColliding = [](auto& bullet, auto& target) {
        auto [bx, by] = bullet.GetPosition(true);  
        auto [tx, ty] = target.GetPosition(false);
        if (bx == tx && by == ty) {
            bullet.DeactivateBullet();
            return true;
        }
        return false;
        };

    for (auto& player : players) {
        auto& movement = player->GetMovementObject();

        if (!movement.IsBulletActive()) {
            continue;
        }
        auto [bulletX, bulletY] = movement.GetPosition(true);

        for (auto& targetPlayer : players) {
            if (player != targetPlayer) {
                auto& targetMovement = targetPlayer->GetMovementObject();

                if (deactivateIfColliding(movement, targetMovement)) {
                    targetPlayer->LoseLife();
                    if (!targetPlayer->HasLife()) {
                        m_playerManager->RemovePlayer(targetPlayer->GetName());
                    }
                }
            }
        }
    }
}
