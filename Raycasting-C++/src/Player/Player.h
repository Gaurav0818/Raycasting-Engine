#pragma once
#include <SDL_render.h>
#include <memory>
#include <cmath>

#include "../Map/Map.h"

class Player
{
public:
    Player();
	~Player() = default;
    
    Player(double x, double y, double width, double height,
        int turnDirection, int walkDirection, double rotationAngle,
        double moveSpeed, double rotationSpeed)
        : m_playerX(x), m_playerY(y), m_width(width), m_height(height),
        m_turnDirection(turnDirection), m_walkDirection(walkDirection),
        m_rotationAngle(rotationAngle), m_moveSpeed(moveSpeed), m_rotationSpeed(rotationSpeed)
    {}

    void RenderPlayer(SDL_Renderer* renderer);

    void MovePlayer(double deltaTime, std::unique_ptr<Map>&  map);
    
public:
    double m_playerX;
    double m_playerY;
    double m_width;
    double m_height;
    int m_turnDirection; // -1 for left, +1 for right
    int m_walkDirection; // -1 for back, +1 for front
    double m_rotationAngle;
    double m_moveSpeed;
    double m_rotationSpeed;
};
