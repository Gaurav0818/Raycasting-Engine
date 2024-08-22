#pragma once
#include <SDL_render.h>
#include <cmath>
#include "Map.h"

class Player
{
public:
    Player();
	~Player() = default;
    
    Player(float x, float y, float width, float height,
        int turnDirection, int walkDirection, float rotationAngle,
        float moveSpeed, float rotationSpeed)
        : x(x), y(y), width(width), height(height),
        turnDirection(turnDirection), walkDirection(walkDirection),
        rotationAngle(rotationAngle), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
    {}

    void InitPlayer(float x, float y, float width, float height,
        int turnDirection, int walkDirection, float rotationAngle,
        float moveSpeed, float rotationSpeed);

    void RenderPlayer(SDL_Renderer* renderer);

    void MovePlayer(float deltaTime, Map* map);
    
public:
    float x;
    float y;
    float width;
    float height;
    int turnDirection; // -1 for left, +1 for right
    int walkDirection; // -1 for back, +1 for front
    float rotationAngle;
    float moveSpeed;
    float rotationSpeed;
};
