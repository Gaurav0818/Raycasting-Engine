#pragma once
#include <SDL_render.h>
#include <cmath>
#include "Constant.h"
#include "Map.h"

class Player
{
public:
    Player();
	~Player() = default;
    
    Player(double x, double y, double width, double height,
        int turnDirection, int walkDirection, double rotationAngle,
        double moveSpeed, double rotationSpeed)
        : x(x), y(y), width(width), height(height),
        turnDirection(turnDirection), walkDirection(walkDirection),
        rotationAngle(rotationAngle), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
    {}

    void RenderPlayer(SDL_Renderer* renderer);

    void MovePlayer(double deltaTime, Map* map);
    
public:
    double x;
    double y;
    double width;
    double height;
    int turnDirection; // -1 for left, +1 for right
    int walkDirection; // -1 for back, +1 for front
    double rotationAngle;
    double moveSpeed;
    double rotationSpeed;
};
