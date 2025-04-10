#include <unordered_map>
#include <string>
#include "Game.h"

class GameManager {
public:
    static bool GameExists(const std::string& gameCode);
    void AddGame(const std::string& gameCode, const Game& game);
    Game& GetGame(const std::string& gameCode);

private:
    static std::unordered_map<std::string, Game> m_games; 
};
