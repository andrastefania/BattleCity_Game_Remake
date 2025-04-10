#pragma once
#include "GameObject.h"

class MovementObject {
public:
    enum class Direction { Up, Down, Left, Right };

    explicit MovementObject(GameObject& gameObject);
    MovementObject(const MovementObject& other);            
    MovementObject(MovementObject&& other) noexcept;        
    MovementObject& operator=(const MovementObject& other); 
    MovementObject& operator=(MovementObject&& other) noexcept;


    // Getters
    std::pair<uint16_t, uint16_t> GetPosition(bool forBullet = false) const;
    Direction GetDirection(bool forBullet = false) const;

    //Setters
    void SetPosition(uint16_t x, uint16_t y, bool forBullet = false);
    void SetDirection(Direction direction, bool forBullet = false);

    std::pair<uint16_t, uint16_t> Move(Direction direction, bool forBullet = false);

    // Bullet management
    void Shoot();
    bool IsBulletActive() const;
    void DeactivateBullet();

private:

    bool m_bulletActive;
    uint16_t m_tankX, m_tankY;
    uint16_t m_bulletX, m_bulletY;

    Direction m_tankDirection;
    Direction m_bulletDirection;

    GameObject& m_gameObject;
};
