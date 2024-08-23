#include "Ray.h"

#include <iostream>

double DistanceBetweenPoints(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double NormalizeAngle(double angle)
{
    angle = std::remainder(angle, static_cast<double>(TWO_PI));
    if (angle < 0) {
        angle = static_cast<double>(TWO_PI) + angle;
    }
    return angle;
}

void Ray::CastRay(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map)
{
    m_rayAngle = NormalizeAngle(angle);

    m_isRayFacingDown = m_rayAngle > 0 && m_rayAngle < PI;
    m_isRayFacingUp = !m_isRayFacingDown;

    m_isRayFacingRight = m_rayAngle < 0.5 * PI || m_rayAngle > 1.5 * PI;
    m_isRayFacingLeft = !m_isRayFacingRight;
    
    double xintercept, yintercept;
    double xstep, ystep;
    

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    double horzWallHitX = 0;
    double horzWallHitY = 0;
    int horzWallContent = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player->m_playerY / TILE_SIZE) * TILE_SIZE;
    yintercept += m_isRayFacingDown ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player->m_playerX + (yintercept - player->m_playerY) / tan(m_rayAngle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;  
    ystep *= m_isRayFacingUp ? -1 : 1;

    xstep = TILE_SIZE / tan(m_rayAngle);
    xstep *= (m_isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (m_isRayFacingRight && xstep < 0) ? -1 : 1;

    double nextHorzTouchX = xintercept;
    double nextHorzTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorzTouchX >= 0 && nextHorzTouchX <= (MAP_NUM_COLS * TILE_SIZE) && nextHorzTouchY >= 0 && nextHorzTouchY <= (MAP_NUM_ROWS * TILE_SIZE))
    {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (m_isRayFacingUp ? -1 : 0);
        if (map->HasWallAt(xToCheck, yToCheck)) {
            foundHorzWallHit = true;
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = map->m_grid[static_cast<int>(yToCheck / TILE_SIZE)][static_cast<int>(xToCheck / TILE_SIZE)];
            break;
        } else {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }
    
    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundVertWallHit = false;
    double vertWallHitX = 0;
    double vertWallHitY = 0;
    int vertWallContent = 0;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player->m_playerX / TILE_SIZE) * TILE_SIZE;
    xintercept += m_isRayFacingRight ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player->m_playerY + (xintercept - player->m_playerX) * tan(m_rayAngle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= m_isRayFacingLeft ? -1 : 1;

    ystep = TILE_SIZE * tan(m_rayAngle);
    ystep *= (m_isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (m_isRayFacingDown && ystep < 0) ? -1 : 1;

    double nextVertTouchX = xintercept;
    double nextVertTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVertTouchX >= 0 && nextVertTouchX <= (MAP_NUM_COLS * TILE_SIZE) && nextVertTouchY >= 0 && nextVertTouchY <= (MAP_NUM_ROWS * TILE_SIZE)) {
        float xToCheck = nextVertTouchX + (m_isRayFacingLeft ? -1 : 0);
        float yToCheck = nextVertTouchY;
        if (map->HasWallAt(xToCheck, yToCheck)) {
            foundVertWallHit = true;
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallContent = map->m_grid[static_cast<int>(yToCheck / TILE_SIZE)][static_cast<int>(xToCheck / TILE_SIZE)];
            break;
        } else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical distances and choose the smallest value
    double horzHitDistance = foundHorzWallHit
        ? DistanceBetweenPoints(player->m_playerX, player->m_playerY, horzWallHitX, horzWallHitY)
        : INT_MAX;
    double vertHitDistance = foundVertWallHit
        ? DistanceBetweenPoints(player->m_playerX, player->m_playerY, vertWallHitX, vertWallHitY)
        : INT_MAX;

    m_wasHitVertical = (vertHitDistance < horzHitDistance);
    // only store the smallest of the distances
    m_wallHitX = m_wasHitVertical ? vertWallHitX : horzWallHitX;
    m_wallHitY = m_wasHitVertical ? vertWallHitY : horzWallHitY;
    m_distance = m_wasHitVertical ? vertHitDistance : horzHitDistance;
    m_wallHitContent = m_wasHitVertical ? vertWallContent : horzWallContent;
}
