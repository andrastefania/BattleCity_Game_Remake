#include "MovementObject.h"
#include <iostream>

// Constructor
MovementObject::MovementObject(GameObject& gameObject)
    : m_gameObject(gameObject), 
      m_tankX(0), 
      m_tankY(0), 
      m_tankDirection(Direction::Up),
      m_bulletActive(false), 
      m_bulletX(0), 
      m_bulletY(0), 
      m_bulletDirection(Direction::Up) {}

MovementObject::MovementObject(const MovementObject& other)
    : m_gameObject(other.m_gameObject), 
      m_tankX(other.m_tankX), 
      m_tankY(other.m_tankY), 
      m_tankDirection(other.m_tankDirection),
      m_bulletX(other.m_bulletX), 
      m_bulletY(other.m_bulletY), 
      m_bulletDirection(other.m_bulletDirection) {}

MovementObject::MovementObject(MovementObject&& other) noexcept
    : m_gameObject(other.m_gameObject),
      m_tankX(other.m_tankX), 
      m_tankY(other.m_tankY), 
      m_tankDirection(other.m_tankDirection),
      m_bulletX(other.m_bulletX), 
      m_bulletY(other.m_bulletY), 
      m_bulletDirection(other.m_bulletDirection) {}

MovementObject& MovementObject::operator=(const MovementObject& other) {
    if (this == &other) return *this;
    m_tankX = other.m_tankX;
    m_tankY = other.m_tankY;
    m_tankDirection = other.m_tankDirection;
    m_bulletX = other.m_bulletX;
    m_bulletY = other.m_bulletY;
    m_bulletDirection = other.m_bulletDirection;
    return *this;
}
MovementObject& MovementObject::operator=(MovementObject&& other) noexcept {
    if (this == &other) return *this; 
    m_tankX = other.m_tankX;
    m_tankY = other.m_tankY;
    m_tankDirection = other.m_tankDirection;
    m_bulletX = other.m_bulletX;
    m_bulletY = other.m_bulletY;
    m_bulletDirection = other.m_bulletDirection;
    return *this;
}

//Getters
std::pair<uint16_t, uint16_t> MovementObject::GetPosition(bool forBullet) const {
    return forBullet ? std::make_pair(m_bulletX, m_bulletY) : std::make_pair(m_tankX, m_tankY);
}
MovementObject::Direction MovementObject::GetDirection(bool forBullet) const {
    return forBullet ? m_bulletDirection : m_tankDirection;
}

//Setters
void MovementObject::SetPosition(uint16_t x, uint16_t y, bool forBullet) {
    if (forBullet) {
        m_bulletX = x;
        m_bulletY = y;
    }
    else {
        m_tankX = x;
        m_tankY = y;
    }
}
void MovementObject::SetDirection(Direction direction, bool forBullet) {
    if (forBullet) {
        m_bulletDirection = direction;
    }
    else {
        m_tankDirection = direction;
    }
}

std::pair<uint16_t, uint16_t> MovementObject::Move(Direction direction, bool forBullet) {
    uint16_t& x = forBullet ? m_bulletX : m_tankX;
    uint16_t& y = forBullet ? m_bulletY : m_tankY;

    switch (direction) {
    case Direction::Up:    --y; break;
    case Direction::Down:  ++y; break;
    case Direction::Left:  --x; break;
    case Direction::Right: ++x; break;
    }
    if (forBullet) {
        m_bulletDirection = direction;
    }
    else {
        m_tankDirection = direction;
    }
    return { x, y };
}

//Bullet management
void MovementObject::Shoot() {
    if (!m_bulletActive) {
        m_bulletActive = true;
        m_bulletX = m_tankX;
        m_bulletY = m_tankY;
        m_bulletDirection = m_tankDirection;
    }
}

bool MovementObject::IsBulletActive() const {
    return m_bulletActive;
}

void MovementObject::DeactivateBullet() {
    m_bulletActive = false;
}
