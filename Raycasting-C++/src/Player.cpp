#include "Player.h"

Player::Player()
{
    playerX = 0;
    playerY = 0;
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
    this->playerX = x;
    this->playerY = y;
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
            static_cast<int>(playerX) , static_cast<int>(playerY),
            static_cast<int>(width), static_cast<int>(height)
        };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(renderer, playerX, playerY, playerX + std::cos(rotationAngle) * 40, playerY + std::sin(rotationAngle) * 40);
}

void Player::MovePlayer(float deltaTime, Map* map)
{
    rotationAngle += turnDirection * rotationSpeed * deltaTime;
    float moveStep = walkDirection * moveSpeed * deltaTime;

    float newPlayerX = playerX + moveStep * std::cos(rotationAngle);
    float newplayerY = playerY + moveStep * std::sin(rotationAngle);

    if(!map->HasWallAt(newPlayerX, newplayerY))
    {
        playerX = newPlayerX;
        playerY = newplayerY;
    }
}
