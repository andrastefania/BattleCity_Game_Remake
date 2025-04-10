#pragma once
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

#include "Map.h"
#include "PlayerManager.h"

import Bomb;
import Tank;
import Wall;


class CollisionManager {

public:
    CollisionManager(std::shared_ptr<Map> map, std::shared_ptr<PlayerManager> playerManager);

    template <typename T>
        requires std::is_integral_v<T>
    bool IsValidPosition(T x, T y) const;
    std::optional<Map::Cell> GetValidCell(uint16_t x, uint16_t y) const;
    void HandleBulletCollisions();
    void HandleTankCollisions();
    void HandleBulletTankCollisions();
    bool isOccupiedByAnotherTank(const std::vector<std::shared_ptr<Player>>& players, uint16_t x, uint16_t y, const std::shared_ptr<Player>& currentPlayer);

    std::shared_ptr<Map> m_map;
    std::shared_ptr<PlayerManager> m_playerManager;
};