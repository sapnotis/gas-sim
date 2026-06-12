#ifndef _PHYSGRAPH_TOOLS_
#define _PHYSGRAPH_TOOLS_

#include <SFML/Graphics.hpp>

struct Vector3d
{
    double x;
    double y;
    double z;

    Vector3d operator+(const Vector3d& other) const {
        return {x + other.x, y + other.y, z + other.z};
    };

    Vector3d operator-(const Vector3d& other) const {
        return {x - other.x, y - other.y, z - other.z};
    };

    Vector3d& operator+=(const Vector3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    };

    Vector3d& operator-=(const Vector3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    };

    Vector3d& operator*=(const double scale) {
        x *= scale; y *= scale; z *= scale;
        return *this;
    };

    Vector3d operator*(const double scale) const {
        return {x*scale, y*scale, z*scale};
    };
};

double drand();
Vector3d rnd_Vector3d_direction();

double len_squared(Vector3d vec);

sf::Color depth_shading(float depth_0to1, const sf::Color& color);

float perspective_multiplier(double z);
void display_point(sf::RenderWindow& window, sf::Vector2f window_center, Vector3d coords, float RadiusInPixels, sf::Color color);
void display_line(sf::RenderWindow& window, sf::Vector2f window_center, Vector3d c1, Vector3d c2, sf::Color col1, sf::Color col2);

#endif