#ifndef _WALLS_LOGIC_
#define _WALLS_LOGIC_

#include "model.hpp"
#include "tools.hpp"
#include <list>

enum class orthogonal_axis {
    X,
    Y,
    Z
};

class Particle;

class RectangularWall // no polymorph today
{
public:
    Point3d midpoint;
    orthogonal_axis axis;
    double radius;
    double velocity;
    
    RectangularWall(Point3d midpoint, orthogonal_axis axis, double radius, double velocity) : midpoint(midpoint), axis(axis), radius(radius), velocity(velocity) { };
    ~RectangularWall() { };

    bool does_collide(const Vector3d& trace);
    void update_coords(std::list<Particle>& particles);
};

#endif