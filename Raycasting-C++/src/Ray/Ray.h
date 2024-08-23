#pragma once
#include <SDL_render.h>

#include "../Player/Player.h"

class Ray
{
public:
    Ray()
        : m_rayAngle(0), m_wallHitX(0), m_wallHitY(0), m_distance(0),
        m_wasHitVertical(false), m_isRayFacingUp(false), m_isRayFacingDown(false),
        m_isRayFacingLeft(false), m_isRayFacingRight(false)
    {}
    ~Ray() = default;

    void CastRay(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map);
    
public:
    double m_rayAngle;
    double m_wallHitX;
    double m_wallHitY;
    double m_distance;
    bool m_wasHitVertical;
    bool m_isRayFacingUp;
    bool m_isRayFacingDown;
    bool m_isRayFacingLeft;
    bool m_isRayFacingRight;
    int m_wallHitContent;
};
