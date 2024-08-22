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

void Player::RenderPlayer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect =
    {
        static_cast<int>(MINIMAP_SCALE_FACTOR * x),
        static_cast<int>(MINIMAP_SCALE_FACTOR * y),
        static_cast<int>(MINIMAP_SCALE_FACTOR * width),
        static_cast<int>(MINIMAP_SCALE_FACTOR * height)
    };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(renderer,
        static_cast<int>(MINIMAP_SCALE_FACTOR * x),
        static_cast<int>(MINIMAP_SCALE_FACTOR * y),
        static_cast<int>(MINIMAP_SCALE_FACTOR * (x + std::cos(rotationAngle) * 40)),
        static_cast<int>(MINIMAP_SCALE_FACTOR * (y + std::sin(rotationAngle) * 40))
    );
}

void Player::MovePlayer(double deltaTime, Map* map)
{
    rotationAngle += turnDirection * rotationSpeed * deltaTime;
    double moveStep = walkDirection * moveSpeed * deltaTime;

    double newPlayerX = x + moveStep * std::cos(rotationAngle);
    double newPlayerY = y + moveStep * std::sin(rotationAngle);

    if(!map->HasWallAt(newPlayerX, newPlayerY))
    {
        x = newPlayerX;
        y = newPlayerY;
    }
}
