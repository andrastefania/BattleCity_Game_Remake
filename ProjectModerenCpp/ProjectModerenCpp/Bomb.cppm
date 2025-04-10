export module Bomb;

import <cstdint>;    
import <iostream>;   

export struct Bomb {
    Bomb(uint16_t x, uint16_t y);

    uint16_t getX() const;
    uint16_t getY() const;
    void setX(const uint16_t& x);
    void setY(const uint16_t& y);

    void explode();
    bool isExploded() const;

private:
    uint16_t m_x;
    uint16_t m_y;
    bool m_exploded = false;
};


Bomb::Bomb(uint16_t x, uint16_t y)
    : m_x(x), m_y(y) {
}

// Getter methods
uint16_t Bomb::getX() const {
    return m_x;
}

uint16_t Bomb::getY() const {
    return m_y;
}

// Setter methods
void Bomb::setX(const uint16_t& x) {
    m_x = x;
}

void Bomb::setY(const uint16_t& y) {
    m_y = y;
}

void Bomb::explode() {
    if (!m_exploded) {
        m_exploded = true;
    }
}

bool Bomb::isExploded() const {
    return m_exploded;
}

