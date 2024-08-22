#include "Ray.h"

#include <iostream>

float DistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float NormalizeAngle(float angle)
{
    angle = std::remainder(angle, static_cast<float>(TWO_PI));
    if (angle < 0) {
        angle = static_cast<float>(TWO_PI) + angle;
    }
    return angle;
}

void Ray::CastRay(float angle, Player* player, Map* map)
{
    rayAngle = NormalizeAngle(angle);

    isRayFacingDown = rayAngle > 0 && rayAngle < PI;
    isRayFacingUp = !isRayFacingDown;

    isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
    isRayFacingLeft = !isRayFacingRight;
    
    float xintercept, yintercept;
    float xstep, ystep;
    

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;

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

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

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
    float vertWallHitX = 0;
    float vertWallHitY = 0;

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

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

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
    float horzHitDistance = foundHorzWallHit
        ? DistanceBetweenPoints(player->x, player->y, horzWallHitX, horzWallHitY)
        : INT_MAX;
    float vertHitDistance = foundVertWallHit
        ? DistanceBetweenPoints(player->x, player->y, vertWallHitX, vertWallHitY)
        : INT_MAX;

    wasHitVertical = (vertHitDistance < horzHitDistance);
    // only store the smallest of the distances
    wallHitX = wasHitVertical ? vertWallHitX : horzWallHitX;
    wallHitY = wasHitVertical ? vertWallHitY : horzWallHitY;
    distance = wasHitVertical ? vertHitDistance : horzHitDistance;
    std::cout << "Vertical Hit: " << foundVertWallHit << " Horizontal Hit: " << foundHorzWallHit << " Distance: " << distance << std::endl;
}

void Ray::RenderRay(SDL_Renderer* renderer, Player* player)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(
        renderer,
        MINIMAP_SCALE_FACTOR * player->x,
        MINIMAP_SCALE_FACTOR * player->y,
        MINIMAP_SCALE_FACTOR * wallHitX,
        MINIMAP_SCALE_FACTOR* wallHitY);
}
