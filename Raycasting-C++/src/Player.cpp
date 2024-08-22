#include "Player.h"

Player::Player()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    turnDirection = 0;
    walkDirection = 0;
    rotationAngle = 0;
    moveSpeed = 0;
    rotationSpeed = 0;
}

void Player::InitPlayer(float x, float y, float width, float height,
    int turnDirection, int walkDirection, float rotationAngle,
    float moveSpeed, float rotationSpeed)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->turnDirection = turnDirection;
    this->walkDirection = walkDirection;
    this->rotationAngle = rotationAngle;
    this->moveSpeed = moveSpeed;
    this->rotationSpeed = rotationSpeed;
}

void Player::RenderPlayer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect =
        {
            MINIMAP_SCALE_FACTOR * static_cast<int>(x) ,
            MINIMAP_SCALE_FACTOR * static_cast<int>(y),
            MINIMAP_SCALE_FACTOR * static_cast<int>(width),
            MINIMAP_SCALE_FACTOR * static_cast<int>(height)
        };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(renderer,
        MINIMAP_SCALE_FACTOR * x,
        MINIMAP_SCALE_FACTOR * y,
        MINIMAP_SCALE_FACTOR * (x + std::cos(rotationAngle) * 40),
        MINIMAP_SCALE_FACTOR * (y + std::sin(rotationAngle) * 40)
        );
}

void Player::MovePlayer(float deltaTime, Map* map)
{
    rotationAngle += turnDirection * rotationSpeed * deltaTime;
    float moveStep = walkDirection * moveSpeed * deltaTime;

    float newPlayerX = x + moveStep * std::cos(rotationAngle);
    float newplayerY = y + moveStep * std::sin(rotationAngle);

    if(!map->HasWallAt(newPlayerX, newplayerY))
    {
        x = newPlayerX;
        y = newplayerY;
    }
}
