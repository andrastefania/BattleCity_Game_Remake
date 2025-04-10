#include "Game.h"
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Validation.h"

#include "CollisionManager.h"
#include "libs/nlohmann/json.hpp"
using json = nlohmann::json;

Game::Game(uint8_t level, const std::string& code)
    : m_map(level),
    m_playerManager(std::make_shared<PlayerManager>()),
    m_gameCode(code),
    m_gameStarted(false) {
    m_collision = std::make_shared<CollisionManager>(std::make_shared<Map>(m_map), m_playerManager);
}
std::string Game::GenerateGameCode()
{
    std::string code;
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 6; ++i) {
        code += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return code;
}
bool Game::IsGameStarted() const
{
    return m_gameStarted;
}

//Getters
Map Game::GetMap() const
{
    return m_map;
}
std::shared_ptr<Player> Game::GetPlayer(const std::string& name)
{
    return m_playerManager->GetPlayer(name);
}

//Player Manager 
void Game::AddPlayer(const std::shared_ptr<Player>& player) {

    std::thread playerThread(&Game::HandlePlayerActions, this, player);
    playerThread.detach();
    player->AddPlayerObject();
    m_playerManager->AddPlayer(player);
   // m_players.emplace(player);
    PlacePlayerOnMap(player);
}
void Game::PlacePlayerOnMap(const std::shared_ptr<Player>& player) {
    static std::vector<std::pair<uint16_t, uint16_t>> cornerPositions = {
        {1, 1},
        {1, m_map.GetHeight() - 2},
        {m_map.GetWidth() - 2, 1},
        {m_map.GetWidth() - 2, m_map.GetHeight() - 2}
    };
    static size_t nextCornerIndex = 0;

    auto [startX, startY] = cornerPositions[nextCornerIndex];
    nextCornerIndex = (nextCornerIndex + 1) % cornerPositions.size();

    if (!Validation::IsValidPosition(startX, startY, m_map.GetWidth(), m_map.GetHeight()))
        std::cout << "erroare la valid";
    if (!std::holds_alternative<Map::Empty>(m_map.GetCell(startX, startY).content)) {
        std::cout << "eroare la cell";
    }

    auto& movement = player->GetMovementObject();
    movement.SetPosition(startX, startY);
    m_map.SetCellContent(startX, startY, Map::Tank{});

    std::pair<int, int> lastCoord = { startX, startY };
    std::pair<int, int> newCoord = { startX, startY };

    RecordChange(newCoord, lastCoord, "player");
    
}
void Game::MovePlayer(const std::string& Nameplayer, MovementObject::Direction direction) {

    auto player = m_playerManager->GetPlayer(Nameplayer);
    auto& movement = player->GetMovementObject();
    auto [currentX, currentY] = movement.GetPosition();
    auto [newX, newY] = movement.Move(direction);

    if (!Validation::IsValidPosition(newX, newY, m_map.GetWidth(), m_map.GetHeight())) {
        std::cout << "Movement blocked to (" << newX << ", " << newY << ") due to invalid position.\n";
        return;
    }

    if (std::holds_alternative<Map::Empty>(m_map.GetCell(newX, newY).content) &&
        !m_collision->isOccupiedByAnotherTank(m_playerManager->GetAllPlayers(), newX, newY, player)) {
        m_map.SetCellContent(currentX, currentY, Map::Empty{});
        m_map.SetCellContent(newX, newY, Map::Tank{});

        movement.SetPosition(newX, newY);
        movement.SetDirection(direction);

        std::pair<int, int> lastCoord = { currentX, currentY };
        std::pair<int, int> newCoord = { newX, newY };

        RecordChange(newCoord, lastCoord, "player");
    }
    if (std::holds_alternative<Wall::TypeWall>(m_map.GetCell(newX, newY).content)) {
        auto& wall = std::get<Wall::TypeWall>(m_map.GetCell(newX, newY).content);
        if (wall == Wall::TypeWall::destructible) {
            m_map.SetCellContent(currentX, currentY, Map::Empty{});
            m_map.SetCellContent(newX, newY, Map::Tank{});
            std::pair<int, int> lastCoord = { currentX, currentY };
            std::pair<int, int> newCoord = { newX, newY };

            RecordChange(newCoord, lastCoord, "wall");
        }
        else
        {
            m_map.SetCellContent(currentX, currentY, Map::Tank{});
            m_map.SetCellContent(newX, newY, Wall::TypeWall::indestructible);
        }
    }
    /*else {
        std::cout << "Movement blocked to (" << newX << ", " << newY << ") due to collision or obstruction.\n";
    }*/
}
void Game::HandlePlayerActions(std::shared_ptr<Player> player)
{
    while (player->IsAlive()) {
        if (player->IsMoving()) {
            player->Move();
        }

        if (player->IsShooting()) {
            player->ShootBulletS();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
bool Game::IsLastPlayer(const std::string& playerName) const {
    std::shared_ptr<Player> player = m_playerManager->GetPlayer(playerName);
    const auto& allPlayers = m_playerManager->GetAllPlayers();
    if (allPlayers.size() == 1 && allPlayers[0] == player) {
        return true;
    }
    return false;
}
bool Game::IsPlayerInGame(const std::string& playerName) const
{
    std::shared_ptr<Player> player = m_playerManager->GetPlayer(playerName);
    return player != nullptr;
}




void Game::ShootBulletS(const std::shared_ptr<Player>& player) {
    auto& movement = player->GetMovementObject();
    movement.Shoot();
}
void Game::MoveBullets() {
    for (const auto& player : m_playerManager->GetAllPlayers()) {
        auto& movement = player->GetMovementObject();
        auto [currentX, currentY] = movement.GetPosition();

        if (movement.IsBulletActive()) {
            auto [newX, newY] = movement.Move(movement.GetDirection(), 1);
            if (Validation::IsValidPosition(newX, newY, m_map.GetWidth(), m_map.GetHeight())) {
                m_collision->HandleBulletCollisions();

                if (std::holds_alternative<Map::Empty>(m_map.GetCell(newX, newY).content)) {
                    m_map.SetCellContent(currentX, currentY, Map::Empty{});
                    m_map.SetCellContent(newX, newY, Map::Bullet{});
                }
                else {
                    movement.DeactivateBullet();
                    m_map.SetCellContent(currentX, currentY, Map::Empty{});
                }
            }
        }
    }

}

//Cell Changes
void Game::RecordChange(const std::pair<int, int>& newCoord, const std::pair<int, int>& lastCoord, const std::string& type)
{
    m_changedCells.push_back({ newCoord, lastCoord, type });
}
const std::vector<std::tuple<std::pair<int, int>, std::pair<int, int>, std::string>>& Game::GetChangedCells() const
{
    return m_changedCells;
}
void Game::ClearChangedCells()
{
    m_changedCells.clear();
}
bool Game::IsMaxPlayersReached() const
{
    return m_playerManager->GetPlayerCount() >= 4;
}
void Game::RemovePlayer(const std::string& playerName)
{
    m_playerManager->RemovePlayer(playerName);
}


void Game::Start() {
    std::cout << "Starting game..." << std::endl;
    m_map.DisplayMap();
}
