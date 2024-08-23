#pragma once

#include "../Player/Player.h"

class Ray
{
public:
    Ray(): m_rayAngle(0), m_wallHitX(0), m_wallHitY(0), m_distance(0), m_wasHitVertical(false) {}
    ~Ray() = default;

    void CastRay(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map);
    
private:
    bool isRayFacingDown() const { return m_rayAngle > 0 && m_rayAngle < PI; }
    bool isRayFacingUp() const { return !isRayFacingDown(); }
    bool isRayFacingRight() const { return m_rayAngle < 0.5 * PI || m_rayAngle > 1.5 * PI; }
    bool isRayFacingLeft() const { return !isRayFacingRight(); }
    
public:
    double m_rayAngle;
    double m_wallHitX;
    double m_wallHitY;
    double m_distance;
    bool m_wasHitVertical;
    int m_wallHitContent;
};
