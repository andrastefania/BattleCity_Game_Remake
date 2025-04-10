#pragma once
#include <iostream>

class Bullet
{
public:
	Bullet();

	void SetBullet(bool active);
	bool IsActive() const; 

	friend std::ostream& operator<<(std::ostream& os, const Bullet& bullet);

private:
	bool m_isActive;
};