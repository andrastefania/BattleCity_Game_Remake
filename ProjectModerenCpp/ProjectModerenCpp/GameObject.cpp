#include "GameObject.h"

GameObject::GameObject() : m_tank(), m_bullet() {}
GameObject::GameObject(Tank&& tank, Bullet&& bullet)
    : m_tank(std::move(tank)), m_bullet(std::move(bullet)) {}

// Getters
const Tank& GameObject::GetTank() const {
    return m_tank;
}

Tank& GameObject::GetTank() {
    return m_tank;
}

const Bullet& GameObject::GetBullet() const {
    return m_bullet;
}

Bullet& GameObject::GetBullet() {
    return m_bullet;
}

void GameObject::ShootBullet() {
    if (!m_bullet.IsActive()) {
        m_bullet.SetBullet(true);
    }
}
