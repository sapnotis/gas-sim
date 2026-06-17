#include "tools.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include <random>

double drand() {

    std::random_device rd; 
    std::mt19937 engine( rd() ); 
    std::uniform_real_distribution<double> dist(-1, 1);
    double value = dist(engine);
    return value;
}

Point3d rnd_Point3d_direction() {

    Point3d vec = { drand(), drand(), drand() };

    // I'm so sorry
    if ( !vec.x && !vec.y && !vec.z )
        vec.x = 1;

    return vec;
}

double len_squared(Point3d vec) {
    return (
        vec.x * vec.x
        + vec.y * vec.y
        + vec.z * vec.z
    );
}

sf::Color depth_shading(float depth_0to1, const sf::Color& color) {
    sf::Color shaded_color = color;
    // shaded_color.r *= 1 - 0.8*depth_0to1;
    // shaded_color.g *= 1 - 0.8*depth_0to1;
    // shaded_color.b *= 1 - 0.8*depth_0to1;
    shaded_color.a = 255 - 255*depth_0to1;
    return shaded_color;
}

const double perspective_distance = 2000.f;

float perspective_multiplier(double z) {
    if ( z >= perspective_distance )
        return 0;
    return ( perspective_distance / ( perspective_distance - z ) );
}

void display_point(sf::RenderWindow& window, sf::Vector2f window_center, Point3d coords, float RadiusInPixels, sf::Color color) {
    
    if ( coords.z >= perspective_distance )
        return;
    
    coords.x *= perspective_multiplier(coords.z);
    coords.y *= perspective_multiplier(coords.z);

    sf::CircleShape circle(RadiusInPixels);
    circle.setOrigin(RadiusInPixels, RadiusInPixels);
    circle.setFillColor(color);
    circle.setPosition( window_center + sf::Vector2f({(float)coords.x, (float)-coords.y}) );
    window.draw(circle);
}

void display_line(sf::RenderWindow& window, sf::Vector2f window_center, Point3d c1, Point3d c2, sf::Color col1, sf::Color col2) {

    c1.x *= perspective_multiplier(c1.z);
    c1.y *= perspective_multiplier(c1.z);
    c2.x *= perspective_multiplier(c2.z);
    c2.y *= perspective_multiplier(c2.z);

    sf::Vertex line[] = {
        sf::Vertex( window_center + sf::Vector2f({(float)c1.x, (float)-c1.y}) ),
        sf::Vertex( window_center + sf::Vector2f({(float)c2.x, (float)-c2.y}) )
    };

    line[0].color = col1;
    line[1].color = col2;
    window.draw(line, 2, sf::Lines);
}