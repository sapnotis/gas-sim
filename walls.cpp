#include "walls.hpp"

#include <map>

bool RectangularWall::does_collide(const Vector3d& trace) {

    int coord_index = (int)axis;

    if ( ( trace.origin[coord_index] - midpoint[coord_index] ) * ( trace.end[coord_index] - midpoint[coord_index] ) < 0 )
        return true;
    return false;
};