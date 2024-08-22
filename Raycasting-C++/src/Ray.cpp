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

void Ray::CastRay(double angle, Player* player, Map* map)
{
    rayAngle = NormalizeAngle(angle);

    isRayFacingDown = rayAngle > 0 && rayAngle < PI;
    isRayFacingUp = !isRayFacingDown;

    isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
    isRayFacingLeft = !isRayFacingRight;
    
    double xintercept, yintercept;
    double xstep, ystep;
    

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    double horzWallHitX = 0;
    double horzWallHitY = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
    yintercept += isRayFacingDown ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player->x + (yintercept - player->y) / tan(rayAngle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;
    ystep *= isRayFacingUp ? -1 : 1;

    xstep = TILE_SIZE / tan(rayAngle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    double nextHorzTouchX = xintercept;
    double nextHorzTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
        if (map->HasWallAt(nextHorzTouchX, nextHorzTouchY - (isRayFacingUp ? 1 : 0))) {
            foundHorzWallHit = true;
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
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

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
    xintercept += isRayFacingRight ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player->y + (xintercept - player->x) * tan(rayAngle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= isRayFacingLeft ? -1 : 1;

    ystep = TILE_SIZE * tan(rayAngle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    double nextVertTouchX = xintercept;
    double nextVertTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
        if (map->HasWallAt(nextVertTouchX - (isRayFacingLeft ? 1 : 0), nextVertTouchY)) {
            foundVertWallHit = true;
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            break;
        } else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical distances and choose the smallest value
    double horzHitDistance = foundHorzWallHit
        ? DistanceBetweenPoints(player->x, player->y, horzWallHitX, horzWallHitY)
        : INT_MAX;
    double vertHitDistance = foundVertWallHit
        ? DistanceBetweenPoints(player->x, player->y, vertWallHitX, vertWallHitY)
        : INT_MAX;

    wasHitVertical = (vertHitDistance < horzHitDistance);
    // only store the smallest of the distances
    wallHitX = wasHitVertical ? vertWallHitX : horzWallHitX;
    wallHitY = wasHitVertical ? vertWallHitY : horzWallHitY;
    distance = wasHitVertical ? vertHitDistance : horzHitDistance;
}

void Ray::RenderRay(SDL_Renderer* renderer, Player* player)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(
        renderer,
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->x),
        static_cast<int>(MINIMAP_SCALE_FACTOR * player->y),
        static_cast<int>(MINIMAP_SCALE_FACTOR * wallHitX),
        static_cast<int>(MINIMAP_SCALE_FACTOR * wallHitY)
    );
}
