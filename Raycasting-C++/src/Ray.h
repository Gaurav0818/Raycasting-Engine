#pragma once
#include <SDL_render.h>

#include "Player.h"

class Ray
{
public:
    Ray()
        : rayAngle(0), wallHitX(0), wallHitY(0), distance(0),
        wasHitVertical(false), isRayFacingUp(false), isRayFacingDown(false),
        isRayFacingLeft(false), isRayFacingRight(false)
    {}
    ~Ray() = default;

    void CastRay(double angle, Player* player, Map* map);
    void RenderRay(SDL_Renderer* renderer,Player* player);
    
public:
    double rayAngle;
    double wallHitX;
    double wallHitY;
    double distance;
    bool wasHitVertical;
    bool isRayFacingUp;
    bool isRayFacingDown;
    bool isRayFacingLeft;
    bool isRayFacingRight;
};
