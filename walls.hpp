#ifndef _WALLS_LOGIC_
#define _WALLS_LOGIC_

#include "tools.hpp"

class Wall
{
private:
    Point3d midpoint;
    bool movable;

public:
    Wall(Point3d midpoint, bool movable) : midpoint(midpoint), movable(movable) { };
    virtual ~Wall() { };

    virtual Vector3d collision_result(Vector3d trace) = 0;
};

class RectangularWall : public Wall
{
private:
    Point3d first_edge;
    Point3d second_edge;
public:
    RectangularWall(Point3d midpoint, bool movable, Point3d first_edge, Point3d second_edge) : Wall(midpoint, movable), first_edge(first_edge), second_edge(second_edge) { };
    ~RectangularWall() { };

    Vector3d collision_result(Vector3d trace) override;
};

#endif