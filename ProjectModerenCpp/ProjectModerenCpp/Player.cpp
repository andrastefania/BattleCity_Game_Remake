#include "Player.h"
#include <iostream>

Player::Player():m_name("default_name"), m_score(0), m_lives(3), m_database(m_database), m_alive(true), m_moving(false), m_shooting(false)
{
}

Player::Player(const std::string& name, Database& db)
    : m_name(name), m_score(0), m_lives(3), m_database(db), m_alive(true), m_moving(false), m_shooting(false) {
    
    if (m_database.ClientExists(m_name)) {
        m_score = m_database.GetScore(m_name);
    }
}

Player::Player(Player&& other) noexcept
    : m_name(std::move(other.m_name)),
    m_score(other.m_score),
    m_lives(other.m_lives),
    m_database(other.m_database), 
    m_moveObject(std::move(other.m_moveObject)) {

}
Player::Player(const Player& other)
    : m_name(other.m_name),
    m_score(other.m_score),
    m_lives(other.m_lives),
    m_database(other.m_database), 
    m_moveObject(other.m_moveObject){
    
}
Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) { // Avoid self-assignment
        m_name = std::move(other.m_name);
        m_score = other.m_score;
        m_lives = other.m_lives;
        m_database = other.m_database;
        m_moveObject = std::move(other.m_moveObject);
    }
    return *this;
}
Player& Player::operator=(const Player& other) {
    if (this != &other) { // Avoid self-assignment
        m_name = other.m_name;
        m_score = other.m_score;
        m_lives = other.m_lives;
        m_database = other.m_database;
        m_moveObject = other.m_moveObject;
    }
    return *this;
}

Player::~Player() {}

void Player::AddPlayerObject()
{
    Tank m_tank;
    Bullet m_bullet;
    GameObject object(std::move(m_tank), std::move(m_bullet));
    m_object = std::move(object);
    m_moveObject.emplace(m_object);
}
void Player::AddMovementObject(MovementObject&& movement) {
    m_moveObject = std::move(movement);
}

//Getters
const std::string& Player::GetName() const {
    return m_name;
}
int Player::GetLives() const {
    return m_lives;
}
int Player::GetScore() const {
    return m_score;
}
GameObject& Player::GetObject()
{
    return m_object;
}
MovementObject& Player::GetMovementObject()
{
    if (!m_moveObject.has_value()) {
        throw std::runtime_error("MovementObject is not initialized!");
    }
    return *m_moveObject;
}

//Setters
void Player::SetMovement(bool moving) {
    m_moving.store(moving); 
}
void Player::SetShooting(bool shooting) {
    m_shooting.store(shooting);
}
void Player::SetAlive(bool alive) {
    m_alive.store(alive);  
}
void Player::SetDirection(MovementObject::Direction direction) {
    m_currentDirection = direction;
}

bool Player::IsAlive() const {
    return m_alive.load();  
}
bool Player::IsMoving() const {
    return m_moving.load() && m_moveObject.has_value(); 
}
bool Player::IsShooting() const {
    return m_shooting.load();
}


//lives management
void Player::LoseLife() {
    if (m_lives > 0) {
        m_lives--;
    }
    else {
        m_hasLife = false;
    }
}
void Player::GainLife() {
    m_lives++;
}
bool Player::HasLife() const
{
    return m_hasLife;
}

//Save state to database 
void Player::SaveState() {
    m_database.UpdateScore(m_name, m_score);
}
void Player::AddScore(int points)
{
    m_score += points;
}
void Player::ChooseKeyBindings(int up, int down, int left, int right, int shoot) {
    if (m_database.SaveKeyBindings(m_name, up, down, left, right, shoot)) {
        std::cout << "Key bindings saved for player " << m_name << "." << std::endl;
    }
    else {
        std::cerr << "Failed to save key bindings for player " << m_name << "." << std::endl;
    }
}

void Player::Move() {
    if (IsAlive() && m_moveObject.has_value()) {
        m_moveObject->Move(m_currentDirection);
    }
}
void Player::ShootBulletS() {
    if (IsAlive()) {
        m_object.ShootBullet();
    }
}

void Player::UpdateDirection(MovementObject::Direction direction)
{
    SetDirection(direction);
    Move();
}