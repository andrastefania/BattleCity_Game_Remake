#include "BonusLife.h"

BonusLife::BonusLife(uint16_t x, uint16_t y)
    : m_x(x), m_y(y) {}

uint16_t BonusLife::getX() const {
    return m_x;
}

uint16_t BonusLife::getY() const {
    return m_y;
}

void BonusLife::setX(const uint16_t& x)
{
    m_x = x;
}
void BonusLife::setY(const uint16_t& y)
{
    m_y = y;
}


void BonusLife::collected() {
    if (m_collected == false) {
        m_collected = true;
    }
}

bool BonusLife::isCollected() const {
    return m_collected;
}
