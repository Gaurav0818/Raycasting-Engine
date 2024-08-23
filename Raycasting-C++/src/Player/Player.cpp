#include "Player.h"

Player::Player()
{
    m_playerX = 0;
    m_playerY = 0;
    m_width = 0;
    m_height = 0;
    m_turnDirection = 0;
    m_walkDirection = 0;
    m_rotationAngle = 0;
    m_moveSpeed = 0;
    m_rotationSpeed = 0;
}

void Player::RenderPlayer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect =
    {
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_playerX),
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_playerY),
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_width),
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_height)
    };
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(renderer,
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_playerX),
        static_cast<int>(MINIMAP_SCALE_FACTOR * m_playerY),
        static_cast<int>(MINIMAP_SCALE_FACTOR * (m_playerX + std::cos(m_rotationAngle) * 40)),
        static_cast<int>(MINIMAP_SCALE_FACTOR * (m_playerY + std::sin(m_rotationAngle) * 40))
    );
}

void Player::MovePlayer(double deltaTime,std::unique_ptr<Map>&  map)
{
    m_rotationAngle += m_turnDirection * m_rotationSpeed * deltaTime;
    double moveStep = m_walkDirection * m_moveSpeed * deltaTime;

    double newPlayerX = m_playerX + moveStep * std::cos(m_rotationAngle);
    double newPlayerY = m_playerY + moveStep * std::sin(m_rotationAngle);

    if(!map->HasWallAt(newPlayerX, newPlayerY))
    {
        m_playerX = newPlayerX;
        m_playerY = newPlayerY;
    }
}
