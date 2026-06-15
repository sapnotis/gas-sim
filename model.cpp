#include "model.hpp"
#include "tools.hpp"
#include "walls.hpp"

#include <vector>
using std::vector;
#include <list>
#include <algorithm>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Model::Model() : yaw(0), pitch(0), scale(100) { }

Model::~Model() { }

Particle* Model::emplace_particle(Point3d coords, Point3d velocity) {   
    return &( particles.emplace_front(coords, velocity) );
}

Wall* Model::emplace_rect_wall(Point3d midpoint, bool movable, Point3d first_edge, Point3d second_edge) {
    return &( walls.emplace_back(midpoint, movable, first_edge, second_edge) );
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

    auto it = walls.begin();
    while ( it != walls.end() && &(*it) != wall )
        it++;
    
    if ( it != walls.end() )
        return 1;

    walls.erase(it);
    return 0;
}

vector<Particle*> Model::getParticles() {
    std::vector<Particle*> particle_ptrs;

    for ( auto it = particles.begin(); it != particles.end(); it++ )
        particle_ptrs.push_back( &(*it) );

    return particle_ptrs;
}

Particle::Particle(Point3d coords, Point3d velocity)
    : coords(coords), velocity(velocity) { }

Particle::~Particle() { }

// V3 and SFML

void Model::tick() {

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ )
        particle->update_coords();
}

void Model::display(sf::RenderWindow& window) {

    if ( particles.empty() )
        return;
    
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
        nodes_window_coords[ &(*particle) ] = calc_window_coords( particle->getCoords(), scale );

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

    display_xyz_axes(window, scale);

    float NodeRadius = 2;
    sf::Color default_color = sf::Color::White;

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ ) {

        sf::Color color = depth_shading(
            ( big_corner.z - nodes_window_coords.at( &(*particle) ).z ) / ( 2*maxdist*scale ),
            default_color
        );

        display_point( window, window_center, nodes_window_coords[ &(*particle) ], NodeRadius, color );
    }

    // for ( auto wall = walls.begin(); wall != walls.end(); wall++ ) {

    //     sf::Color color = depth_shading(
    //         ( big_corner.z - nodes_window_coords.at( &(*particle) ).z ) / ( 2*maxdist*scale ),
    //         default_color
    //     );

    //     display_point( window, window_center, nodes_window_coords[ &(*particle) ], NodeRadius, color );
    // }
}

Point3d Model::calc_window_coords(Point3d coords, float scale) {

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

void Model::display_xyz_axes(sf::RenderWindow& window, float scale) {

    sf::Vector2f xyz_compass_pos = { 0.15f * window.getSize().x, 0.8f * window.getSize().y };
    
    std::map<int, Point3d> others;

    others[0] = calc_window_coords( {0, 0, 0}, scale );
    float dir_len = 100 / scale;
    others[1] = calc_window_coords( {dir_len, 0, 0}, scale );
    others[2] = calc_window_coords( {0, dir_len, 0}, scale );
    others[3] = calc_window_coords( {0, 0, dir_len}, scale );

    display_line( window, xyz_compass_pos, others[0], others[3], sf::Color::Blue, sf::Color::Blue );
    display_line( window, xyz_compass_pos, others[0], others[2], sf::Color::Green, sf::Color::Green );
    display_line( window, xyz_compass_pos, others[0], others[1], sf::Color::Red, sf::Color::Red );
}

void Particle::update_coords() {

    // big brain walls collision
    coords += velocity;
}