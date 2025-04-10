#pragma once
#include <string>
#include <optional>
#include "Database.h"
#include "MovementObject.h"
#include <atomic>


class Player {
public:
   
    Player();
    Player(const std::string& name, Database& db);
    Player(Player&& other) noexcept;         
    Player(const Player& other);             
    ~Player();                               
    Player& operator=(Player&& other) noexcept; 
    Player& operator=(const Player& other);     

    void AddPlayerObject();
    void AddMovementObject(MovementObject&& movement);

    // Getters
    const std::string& GetName() const;
    int GetLives() const;
    int GetScore() const;
    GameObject& GetObject();
    MovementObject& GetMovementObject();

    //Setters
    void SetMovement(bool moving);  
    void SetShooting(bool shooting); 
    void SetAlive(bool alive);
    void SetDirection(MovementObject::Direction direction);

    bool IsAlive() const;  
    bool IsMoving() const; 
    bool IsShooting() const; 

    // Lives management
    void LoseLife();
    void GainLife();
    bool HasLife()const;

    // Save state to database
    void SaveState();
    void AddScore(int points);
    void ChooseKeyBindings(int up, int down,int left, int right, int shoot);

    void Move();  
    void ShootBulletS(); 

    MovementObject::Direction m_currentDirection;
    void UpdateDirection(MovementObject::Direction direction);


private:
    std::string m_name;
    int m_score;
    int m_lives;
    bool m_hasLife=true;
    Database& m_database;

    GameObject m_object;
    std::optional<MovementObject> m_moveObject;

    std::atomic<bool> m_alive;
    std::atomic<bool> m_moving;
    std::atomic<bool> m_shooting;

};
