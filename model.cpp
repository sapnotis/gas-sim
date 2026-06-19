#include "model.hpp"
#include "tools.hpp"
#include "walls.hpp"
#include "txt_handler.hpp"
#include "style_keeper.hpp"

#include <vector>
#include <algorithm>
#include <map>
#include <cmath>

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

Particle* Model::emplace_particle(Point3d coords, Point3d velocity) {   
    return &( particles.emplace_front(coords, velocity) );
}

RectangularWall* Model::emplace_rect_wall(Point3d midpoint, orthogonal_axis axis, double radius, double velocity) {
    return &( rect_walls.emplace_back(midpoint, axis, radius, velocity) );
}

bool Model::erase_particle(Particle* particle) {
    
    auto it = particles.begin();
    while ( it != particles.end() && &(*it) != particle )
        it++;
    
    if ( it != particles.end() )
        return 1;

    particles.erase(it);
    return 0;
}

bool Model::erase_rect_wall(RectangularWall* wall) {

    auto it = rect_walls.begin();
    while ( it != rect_walls.end() && &(*it) != wall )
        it++;
    
    if ( it != rect_walls.end() )
        return 1;

    rect_walls.erase(it);
    return 0;
}

std::vector<Particle*> Model::getParticles() {
    std::vector<Particle*> particle_ptrs;

    for ( auto it = particles.begin(); it != particles.end(); it++ )
        particle_ptrs.push_back( &(*it) );

    return particle_ptrs;
}

std::vector<RectangularWall*> Model::getWalls() {
    std::vector<RectangularWall*> wall_ptrs;

    for ( auto it = rect_walls.begin(); it != rect_walls.end(); it++ )
        wall_ptrs.push_back( &(*it) );

    return wall_ptrs;
}

Particle::Particle(Point3d coords, Point3d velocity)
    : coords(coords), velocity(velocity) { }

Particle::~Particle() { }

// V3 and SFML

void Model::tick() {

    std::cout << calc_pressure() << "\t" << get_volume() << "\t" << calc_temperature() << std::endl;

    Txt_handler::log(
        std::to_string(calc_pressure()) + "\t"
        + std::to_string(get_volume()) + "\t"
        + std::to_string(calc_temperature())
    );
    
    for ( auto wall = rect_walls.begin(); wall != rect_walls.end(); wall++ )
        wall->update_coords(particles);

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ )
        particle->update_coords(rect_walls);
}

void Model::display(sf::RenderWindow& window) {
    
    if ( yaw > 6.28f ) yaw -= 6.28f;
    if ( yaw < -6.28f ) yaw += 6.28f;

    if ( pitch > 1.57f ) pitch = 1.57f;
    if ( pitch < -1.57f ) pitch = -1.57f;
    
    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };



    float maxdist_sqr = 1;
    for ( Particle particle : particles ) {
        float len_sqr = (float)len_squared( particle.getCoords() );
        if ( maxdist_sqr < len_sqr )
            maxdist_sqr = len_sqr;
    }
    float maxdist = std::sqrt(maxdist_sqr);

    // if ( window_center.y < window_center.x )
    //     scale = 0.9f * window_center.y / maxdist;
    // else
    //     scale = 0.9f * window_center.x / maxdist;

    std::map<Particle*, Point3d> nodes_window_coords;

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ )
        nodes_window_coords[ &(*particle) ] = calc_window_coords( particle->getCoords() );

    Point3d small_corner = {0, 0, 0};
    Point3d big_corner = {0, 0, 0};

    for ( auto [n, coords] : nodes_window_coords ) {

        if ( coords.x > big_corner.x )
            big_corner.x = coords.x;
        if ( coords.x < small_corner.x )
            small_corner.x = coords.x;

        if ( coords.y > big_corner.y )
            big_corner.y = coords.y;
        if ( coords.y < small_corner.y )
            small_corner.y = coords.y;
            
        if ( coords.z > big_corner.z )
            big_corner.z = coords.z;
        if ( coords.z < small_corner.z )
            small_corner.z = coords.z;
    }

    // display

    display_xyz_axes(window);

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ ) {

        sf::Color visible_color = depth_shading(
            ( big_corner.z - nodes_window_coords.at( &(*particle) ).z ) / ( 2*maxdist*scale ),
            default_color
        );

        display_point( window, window_center, nodes_window_coords[ &(*particle) ], NodeRadius, visible_color );
    }

    for ( auto rect_wall = rect_walls.begin(); rect_wall != rect_walls.end(); rect_wall++ ) {

        Point3d mid = rect_wall->midpoint;
        Point3d fir, sec;
        sf::Color wall_color = default_color;
        if ( rect_wall->velocity )
            wall_color =  sf::Color::Cyan;

        switch ( static_cast<int>(rect_wall->axis) )
        {
        case 0: {
            fir = Point3d({0, 1, 0}) * rect_wall->radius;
            sec = Point3d({0, 0, 1}) * rect_wall->radius;
            break;
        } case 1: {
            fir = Point3d({0, 0, 1}) * rect_wall->radius;
            sec = Point3d({1, 0, 0}) * rect_wall->radius;
            break;
        } default: {
            fir = Point3d({1, 0, 0}) * rect_wall->radius;
            sec = Point3d({0, 1, 0}) * rect_wall->radius;
            break;
        }
        }

        //  cross-like display

        // display_line( window, window_center,
        //     calc_window_coords( mid + fir ), calc_window_coords( mid - fir ),
        //     wall_color, wall_color
        // );
        // display_line( window, window_center,
        //     calc_window_coords( mid + sec ), calc_window_coords( mid - sec ),
        //     wall_color, wall_color
        // );

        //  rectangle-like display

        display_line( window, window_center,
            calc_window_coords( mid + fir + sec ), calc_window_coords( mid + fir - sec ),
            wall_color, wall_color
        );
        display_line( window, window_center,
            calc_window_coords( mid + fir - sec ), calc_window_coords( mid - fir - sec ),
            wall_color, wall_color
        );
        display_line( window, window_center,
            calc_window_coords( mid - fir - sec ), calc_window_coords( mid - fir + sec ),
            wall_color, wall_color
        );
        display_line( window, window_center,
            calc_window_coords( mid - fir + sec ), calc_window_coords( mid + fir + sec ),
            wall_color, wall_color
        );
    }
}

Point3d Model::calc_window_coords(Point3d coords) {

    Point3d yaw_coords = {
        coords.x * std::cos( yaw ) - coords.z * std::sin( yaw ),
        coords.y,
        coords.x * std::sin( yaw ) + coords.z * std::cos( yaw )
    };

    Point3d final_coords = {
        yaw_coords.x,
        yaw_coords.y * std::cos( pitch ) - yaw_coords.z * std::sin( pitch ),
        yaw_coords.y * std::sin( pitch ) + yaw_coords.z * std::cos( pitch )
    };

    final_coords.x = final_coords.x * scale;
    final_coords.y = final_coords.y * scale;
    final_coords.z = final_coords.z * scale;

    return final_coords;
}

void Model::display_xyz_axes(sf::RenderWindow& window) {

    sf::Vector2f xyz_compass_pos = { 0.15f * window.getSize().x, 0.8f * window.getSize().y };
    
    std::map<int, Point3d> others;

    others[0] = calc_window_coords( {0, 0, 0} );
    float dir_len = 100 / scale;
    others[1] = calc_window_coords( {dir_len, 0, 0} );
    others[2] = calc_window_coords( {0, dir_len, 0} );
    others[3] = calc_window_coords( {0, 0, dir_len} );

    display_line( window, xyz_compass_pos, others[0], others[3], sf::Color::Blue, sf::Color::Blue );
    display_line( window, xyz_compass_pos, others[0], others[2], sf::Color::Green, sf::Color::Green );
    display_line( window, xyz_compass_pos, others[0], others[1], sf::Color::Red, sf::Color::Red );
}

double Model::get_kinenergy() {
    
    double kinenergy = 0;

    double half_mass = Txt_handler::PARTICLE_MASS / 2;
    for ( Particle particle : particles )
        kinenergy += half_mass * len_squared( particle.getVelocity() );

    return kinenergy;
}

double Model::get_volume() {
    
    double volume = 1;

    volume *= rect_walls[0].midpoint[0] - rect_walls[1].midpoint[0];
    volume *= rect_walls[2].midpoint[1] - rect_walls[3].midpoint[1];
    volume *= rect_walls[4].midpoint[2] - rect_walls[5].midpoint[2];

    return volume;
}

double Model::calc_temperature() {

    // U = 3/2 * N * k_B * T
    double temperature = get_kinenergy() / ( 1.5 * particles.size() * Txt_handler::k_B );

    return temperature;
}

double Model::calc_pressure() {

    // P = n * k_B * T
    // double pressure = ( particles.size() / get_volume() ) * Txt_handler::k_B * calc_temperature();

    // U = 3/2 * (P * V) <--- quicker
    double pressure = get_kinenergy() / ( 1.5 * get_volume() );

    return pressure;
}

void Particle::update_coords(vector<RectangularWall>& walls) {

    Vector3d track = { coords, coords += velocity * Txt_handler::dt };

    bool may_collide = true;
    int collision_counter = 0;

    while ( may_collide && collision_counter < 1024 ) {

        may_collide = false;

        for ( RectangularWall wall : walls ) {
            int coord_index = (int)(wall.axis);

            if ( wall.does_collide(track) ) {
                may_collide = true;
                collision_counter++;
                velocity[coord_index] = 2*wall.velocity - velocity[coord_index];
                coords[coord_index] = 2*( wall.midpoint[coord_index] ) - coords[coord_index];
                track.end = coords;
            }
        }
    }
}