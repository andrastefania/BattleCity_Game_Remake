#include "Map.h"
#include <cstdlib>
#include <ctime>

Map::Map(uint8_t level) {
    auto [minWidth, maxWidth, numBombs, numBonusLives] = GetLevelBounds(level);

    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed the random generator

    m_width = minWidth + std::rand() % (maxWidth - minWidth + 1);
    m_height = minWidth + std::rand() % (maxWidth - minWidth + 1);

    // Resize the map and initialize each cell
    m_map.resize(m_height, std::vector<Cell>(m_width));

    for (uint16_t y = 0; y < m_height; ++y) {
        for (uint16_t x = 0; x < m_width; ++x) {
            if (y == 0 || y == m_height - 1 || x == 0 || x == m_width - 1) {
                m_map[y][x] = { Wall::TypeWall::indestructible, 1 }; // Borders
            }
            else {
                m_map[y][x] = { Empty{}, 0 }; // Inner cells start as empty
            }
        }
    }
    GenerateWalls(level);
    InitializeGameElements(numBombs, numBonusLives);

}

// Getters
uint16_t Map::GetWidth() const { return m_width; }
uint16_t Map::GetHeight() const { return m_height; }
const Map::Cell& Map::GetCell(uint16_t x, uint16_t y) const {
    if (!Validation::IsValidPosition(x, y, m_width, m_height)) {
        throw std::out_of_range("Invalid position on the map!");
    }
    return m_map[y][x];
}
const std::vector<std::vector<Map::Cell>>& Map::GetMap() const {
    return m_map;
}


// Setters
void Map::SetCell(uint16_t x, uint16_t y, const Cell& value) {
    if (!Validation::IsValidPosition(x, y, m_width, m_height)) {
        throw std::out_of_range("Invalid position on the map!");
    }
    m_map[y][x] = value;
}

void Map::SetCellContent(uint16_t x, uint16_t y, CellContent content) {
    if (!Validation::IsValidPosition(x, y, m_width, m_height)) {
        throw std::out_of_range("Invalid position on the map!");
    }
    m_map[y][x].content = content;
}

// Validation
//bool Map::IsValidPosition(uint16_t x, uint16_t y) const {
//    return x < m_width && y < m_height;
//}

std::pair<uint16_t, uint16_t> Map::FindValidPosition() {
    uint16_t x, y;
    auto isCorner = [this](uint16_t x, uint16_t y) {
        return (x == 1 && y == 1) ||                                    
            (x == 1 && y == m_width - 2) ||                          
            (x == m_height - 2 && y == 1) ||                         
            (x == m_height - 2 && y == m_width - 2);                 
        };
    do {
        x = std::rand() % m_width;
        y = std::rand() % m_height;
    } while (!Validation::IsValidPosition(x, y, m_width, m_height) || !std::holds_alternative<Empty>(m_map[y][x].content) || isCorner(x, y));

    return { x, y };
}

// Game Element Initialization
void Map::InitializeGameElements(uint8_t numBombs, uint8_t numBonusLives) {
    for (uint8_t i = 0; i < numBombs; ++i) {
        auto [x, y] = FindValidPosition();
        SetCellContent(x, y, Bomb{});
    }

    for (uint8_t i = 0; i < numBonusLives; ++i) {
        auto [x, y] = FindValidPosition();
        SetCellContent(x, y, BonusLife{});
    }
}


// Level Bounds
std::tuple<uint16_t, uint16_t, uint16_t, uint16_t> Map::GetLevelBounds(uint8_t level) const {
    switch (level) {
    case 1: return { 15, 20, 1, 1 }; // Width range and game elements for level 1
    case 2: return { 20, 25, 2, 2 }; // Level 2
    case 3: return { 25, 30, 3, 3 }; // Level 3
    default:
        throw std::invalid_argument("Invalid level! Levels must be 1, 2, or 3.");
    }
}

// Placing Game Elements
void Map::PlaceBomb(uint16_t x, uint16_t y) { SetCellContent(x, y, Bomb{}); }
void Map::PlaceBonusLife(uint16_t x, uint16_t y) { SetCellContent(x, y, BonusLife{}); }
void Map::PlaceTank(uint16_t x, uint16_t y, const Tank& tank) { SetCellContent(x, y, tank); }

// Display
void Map::DisplayMap() const {
    for (const auto& row : m_map) {
        for (const auto& cell : row) {
            std::visit([](const auto& content) {
                using T = std::decay_t<decltype(content)>;
                if constexpr (std::is_same_v<T, Empty>) {
                    std::cout << "_";
                }
                else if constexpr (std::is_same_v<T, Bomb>) {
                    std::cout << "B";
                }
                else if constexpr (std::is_same_v<T, BonusLife>) {
                    std::cout << "L";
                }
                else if constexpr (std::is_same_v<T, Wall::TypeWall>) {
                    std::cout << (content == Wall::TypeWall::destructible ? '@' : '#');
                }
                else if constexpr (std::is_same_v<T, Tank>) {
                    std::cout << "T";
                }
                else if constexpr (std::is_same_v<T, Bullet>) {
                    std::cout << "->";
                }
                }, cell.content);
        }
        std::cout << '\n';
    }
}

void Map::GenerateWalls(uint8_t level) {
    uint16_t height = GetHeight();
    uint16_t width = GetWidth();

    std::vector<std::pair<uint16_t, uint16_t>> predefinedIndestructibleWalls;
    std::vector<std::pair<uint16_t, uint16_t>> predefinedDestructibleWalls;

    switch (level) {
    case 1:
        predefinedIndestructibleWalls = {
            {0, 6}, {1, 6}, {1, 7}, {3, 8}, {4, 0}, {4, 1}, {4, 8},
            {5, 2}, {5, 8}, {5, 9}, {5, 10}, {5, 14}, {6, 6}, {6, 14},
            {7, 5}, {7, 6}, {7, 7}, {10, 0}, {10, 1}, {10, 2}, {10, 3},
            {10, 11}, {10, 12}, {12, 5}, {12, 9}, {12, 10}, {12, 11},
            {12, 12}, {13, 5}, {13, 12}, {14, 5}
        };
        predefinedDestructibleWalls = {
            {2, 2}, {2, 6}, {2, 9}, {6, 9}, {8, 8}, {11, 4}, {11, 9},
            {13, 7}, {14, 10}
        };
        break;
    case 2:
        predefinedIndestructibleWalls = {
            {2, 1}, {2, 2}, {2, 3}, {0, 7}, {0, 8}, {0, 9}, {1, 8}, {2, 8},
            {3, 8}, {4, 12}, {1, 16}, {2, 16}, {6, 6}, {6, 7}, {6, 8},
            {6, 9}, {7, 13}, {8, 13}, {9, 13}
        };
        predefinedDestructibleWalls = {
            {3, 7}, {3, 10}, {7, 10}, {8, 9}, {9, 8}, {10, 8}, {12, 8}
        };
        break;
    case 3:
        predefinedIndestructibleWalls = {
      {2, 9}, {2, 10}, {2,17},{2,18},{2,19},{2,20},
     {2, 21}, {3, 9}, {3, 10},{3,19}, { 4, 11 }, {4, 12},{15,16} ,{ 5, 24 },
     {6,1}, { 6, 24 },{7,9},{7, 24}, {8, 3}, {8, 4},{8,8}, { 9, 4 },{9,18},
     { 10, 4 },{10,11},{10,18},{10,19},{11,17},{11,18},{11,20},{11,21},{12,21},
     {14,20},{14,21},{15,14},{16,1},{16,12},{16,13},{16,14},{17,14}, { 18,5 },
     {18,6},{18,7},{18,8},{18,14},{19,4},{ 19,5 },{19,22},{19,23},{20,6},{20,14},
     {20,15},{20,24},{21,14},{21,24},{22,5},{22,24},{24,8},{24,21}
        };
        predefinedDestructibleWalls = {
            {1,2},{1,3},{3,18}, { 6, 10 },{6,20}, {9, 7}, {10, 6}, {11, 5},{11,19}, { 12, 4 },
            {13,21},{14,10},{14,11},{15,10},{15,11},{15,18},{15,19},{15,24},{16,4},{16,11},{17,3},{18,3},{18,4},
            {18,13},{19,3},{19,13},{19,14},{19,15},{19,24},{20,23},{21,6},{21,15},{22,15},{23,13},{23,14},
            {23,15},{23,16},{23,19},{24,1},{24,14}
        };
        break;
    default:
        throw std::invalid_argument("Invalid level!");
    }

    for (const auto& pos : predefinedIndestructibleWalls) {
        SetCellContent(pos.first, pos.second, Wall::TypeWall::indestructible);
    }
    for (const auto& pos : predefinedDestructibleWalls) {
        SetCellContent(pos.first, pos.second, Wall::TypeWall::destructible);
    }

    int additionalRandomWalls = 10;
    for (int i = 0; i < additionalRandomWalls; ++i) {
        auto [x, y] = FindValidPosition();
        SetCellContent(x, y, Wall::TypeWall::destructible);
    }
}
