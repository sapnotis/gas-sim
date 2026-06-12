#include "gas.hpp"

#include <vector>
using std::vector;
#include <list>
#include <algorithm>
#include <map>
#include <cmath>

#include <iostream>
#include <SFML/Graphics.hpp>

Gas::Gas() : yaw(0), pitch(0) { }

Gas::~Gas() { }

Particle* Gas::emplace_particle(Vector3d coords, Vector3d velocity) {   
    return &( particles.emplace_front(coords, velocity) );
}

bool Gas::erase_particle(Particle* particle) {
    
    auto it = particles.begin();
    while ( it != particles.end() && &(*it) != particle )
        it++;
    
    if ( it != particles.end() )
        return 1;

    particles.erase(it);
    return 0;
}

vector<Particle*> Gas::getParticles() {
    std::vector<Particle*> particle_ptrs;

    for ( auto it = particles.begin(); it != particles.end(); it++ )
        particle_ptrs.push_back( &(*it) );

    return particle_ptrs;
}

Particle::Particle(Vector3d coords, Vector3d velocity)
    : coords(coords), velocity(velocity) { };

Particle::~Particle() { };

// V3 and SFML

void Gas::tick() {

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ )
        particle->update_coords();
}

void Gas::display(sf::RenderWindow& window) {

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

    float scale;
    if ( window_center.y < window_center.x )
        scale = 0.9f * window_center.y / maxdist;
    else
        scale = 0.9f * window_center.x / maxdist;

    std::map<Particle*, Vector3d> nodes_window_coords;

    for ( auto particle = particles.begin(); particle != particles.end(); particle++ )
        nodes_window_coords[ &(*particle) ] = calc_window_coords( particle->getCoords(), scale );

    Vector3d small_corner = {0, 0, 0};
    Vector3d big_corner = {0, 0, 0};

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
}

Vector3d Gas::calc_window_coords(Vector3d coords, float scale) {

    Vector3d yaw_coords = {
        coords.x * std::cos( yaw ) - coords.z * std::sin( yaw ),
        coords.y,
        coords.x * std::sin( yaw ) + coords.z * std::cos( yaw )
    };

    Vector3d final_coords = {
        yaw_coords.x,
        yaw_coords.y * std::cos( pitch ) - yaw_coords.z * std::sin( pitch ),
        yaw_coords.y * std::sin( pitch ) + yaw_coords.z * std::cos( pitch )
    };

    final_coords.x = final_coords.x * scale;
    final_coords.y = final_coords.y * scale;
    final_coords.z = final_coords.z * scale;

    return final_coords;
}

/*
void Gas::display_grid(sf::RenderWindow& window, sf::Color grid_color) {

    sf::Vector2f window_center = { 0.5f * window.getSize().x, 0.5f * window.getSize().y };

    // circumscribed parallelogram (is this even a word? circumscribed)
    std::map<int, Vector3d> corners;

    // big brain bool logic
    for ( int i = 0; i < 8; i++ ) {
        corners[i] =
            { (i % 2) * small_corner.x + !(i % 2) * big_corner.x,
            (i/2 % 2) * small_corner.y + !(i/2 % 2) * big_corner.y,
            (i/4 % 8) * small_corner.z + !(i/4 % 2) * big_corner.z };
    }

    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[i], corners[i+4], grid_color, grid_color );
    for ( int i = 0; i < 4; i ++ )
        display_line( window, window_center, corners[2*i], corners[2*i+1], grid_color, grid_color );
    for ( int i = 0; i < 2; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );
    for ( int i = 4; i < 6; i ++ )
        display_line( window, window_center, corners[i], corners[i+2], grid_color, grid_color );
}
*/

void Gas::display_xyz_axes(sf::RenderWindow& window, float scale) {

    sf::Vector2f xyz_compass_pos = { 0.15f * window.getSize().x, 0.8f * window.getSize().y };
    
    std::map<int, Vector3d> others;

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

    coords += velocity;
}