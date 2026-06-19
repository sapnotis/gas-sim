#include "walls.hpp"

#include "model.hpp"
#include "tools.hpp"
#include "config_keeper.hpp"
#include "style_keeper.hpp"

bool RectangularWall::does_collide(const Vector3d& trace) {

    int coord_index = (int)axis;

    if ( ( trace.origin[coord_index] - midpoint[coord_index] ) * ( trace.end[coord_index] - midpoint[coord_index] ) < 0 )
        return true;
    return false;
}

void RectangularWall::update_coords(std::list<Particle>& particles) {

    int coord_index = (int)axis;

    midpoint[coord_index] += velocity * Config_keeper::dt;

    for ( auto it = particles.begin(); it != particles.end(); it++ ) {

        Vector3d relative_particle_track;

        relative_particle_track.end = relative_particle_track.origin = it->getCoords();
        relative_particle_track.end[coord_index] += velocity * Config_keeper::dt;

        if ( does_collide(relative_particle_track) ) {

            Point3d vel = it->getVelocity();
            vel[coord_index] = 2*velocity - vel[coord_index];
            it->set_velocity(vel);

            Point3d crd = it->getCoords();
            crd[coord_index] =  2*midpoint[coord_index] - crd[coord_index];
            it->set_coords(crd);
        }
    }
}