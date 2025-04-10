#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <variant>
#include <tuple>
#include <stdexcept>

#include "Validation.h"

import Wall;

class Map {
public:
    struct Empty {};
    struct Bomb {};
    struct Bullet{};
    struct BonusLife {};
    struct Tank {};

    using CellContent = std::variant<Empty, Bomb, BonusLife, Wall::TypeWall, Tank, Bullet>;

    struct Cell {
        CellContent content; 
        uint16_t border;

        Cell() : content(Empty{}), border(0) {}

        Cell(CellContent content, uint16_t border)
            : content(std::move(content)), border(border) {}

        int ToInt() const {
            return std::visit([this](const auto& content) -> int {
                using T = std::decay_t<decltype(content)>;
                if constexpr (std::is_same_v<T, Empty>) return 0;
                else if constexpr (std::is_same_v<T, Bomb>) return 1;
                else if constexpr (std::is_same_v<T, BonusLife>) return 2;
                else if constexpr (std::is_same_v<T, Wall::TypeWall>) {
                    if (border != 0) return 7; 
                    return (content == Wall::TypeWall::indestructible) ? 4 : 3;
                }
                else if constexpr (std::is_same_v<T, Tank>) return 5;
                else if constexpr (std::is_same_v<T, Bullet>) return 6;
                else return -1; 
                }, content);
        }
    };

public:
    explicit Map(uint8_t level);

    // Getters
    uint16_t GetWidth() const;
    uint16_t GetHeight() const;
    const Cell& GetCell(uint16_t x, uint16_t y) const;
    const std::vector<std::vector<Cell>>& GetMap() const;


    // Setters
    void SetCell(uint16_t x, uint16_t y, const Cell& value);
    void SetCellContent(uint16_t x, uint16_t y, CellContent content);

    // Validation
    //bool IsValidPosition(uint16_t x, uint16_t y) const;

    // Add specific elements
    void PlaceBomb(uint16_t x, uint16_t y);
    void PlaceBonusLife(uint16_t x, uint16_t y);
    void PlaceTank(uint16_t x, uint16_t y, const Tank& tank);

    // Display
    void DisplayMap() const;

private:
    uint16_t m_width;
    uint16_t m_height;
    std::vector<std::vector<Cell>> m_map;

    std::tuple<uint16_t, uint16_t, uint16_t, uint16_t> GetLevelBounds(uint8_t level) const;
    std::pair<uint16_t, uint16_t> FindValidPosition();

    void InitializeGameElements(uint8_t numBombs, uint8_t numBonusLives);
    void GenerateWalls(uint8_t level);
};
