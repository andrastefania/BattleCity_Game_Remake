#pragma once
#include <cstdint>
#include "Bullet.h"

import Tank;

class GameObject 
{
public:
    GameObject();
    GameObject(Tank&& tank, Bullet&& bullet= Bullet());

    // Getters
    const Tank& GetTank() const;
    Tank& GetTank(); 
    const Bullet& GetBullet() const;
    Bullet& GetBullet();

    void ShootBullet(); 

private:
    Tank m_tank;
    Bullet m_bullet;
};

