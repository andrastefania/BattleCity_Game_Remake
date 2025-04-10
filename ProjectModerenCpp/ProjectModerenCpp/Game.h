#pragma once
#include <memory>
#include <vector>
#include <array>
#include <thread>
#include <atomic>

#include "Map.h"
#include "PlayerManager.h"
#include "CollisionManager.h"

class Game {
public:
    explicit Game(uint8_t level=1, const std::string& code=GenerateGameCode());
    static std::string GenerateGameCode();
    bool IsGameStarted() const;

    //Getters
    Map GetMap()const;
    std::shared_ptr<Player> GetPlayer(const std::string& name);

    //Player Manager
    void AddPlayer(const std::shared_ptr<Player>& player);
    void PlacePlayerOnMap(const std::shared_ptr<Player>& player);
    void MovePlayer(const std::string& playerName, MovementObject::Direction direction);
    void HandlePlayerActions(std::shared_ptr<Player> player);
    bool IsPlayerInGame(const std::string& playerName) const;
    bool IsLastPlayer(const std::string& playerName) const;

    //Bullet Manager
    void ShootBulletS(const std::shared_ptr<Player>& player);
    void MoveBullets();

    //Cell Changes
    void RecordChange(const std::pair<int, int>& newCoord, const std::pair<int, int>& lastCoord, const std::string& type);
    const std::vector<std::tuple<std::pair<int, int>, std::pair<int, int>, std::string>>& GetChangedCells() const;
    void ClearChangedCells();
    bool IsMaxPlayersReached() const;
    void RemovePlayer(const std::string& playerName);

    void Start();

private:
    Map m_map;
    std::string m_gameCode;
    bool m_gameStarted;
    std::shared_ptr<CollisionManager> m_collision;
    std::shared_ptr<PlayerManager> m_playerManager;
    std::vector<std::tuple<std::pair<int, int>, std::pair<int, int>, std::string>> m_changedCells;
};
