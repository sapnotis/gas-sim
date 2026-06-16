#ifndef _TOOLS_
#define _TOOLS_

#include <SFML/Graphics.hpp>

struct Point3d
{
    double x;
    double y;
    double z;

    const double& operator[](size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Coordinate index out of bounds");
        }
    };

    double& operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Coordinate index out of bounds");
        }
    };

    Point3d operator+(const Point3d& other) const {
        return {x + other.x, y + other.y, z + other.z};
    };

    Point3d operator-(const Point3d& other) const {
        return {x - other.x, y - other.y, z - other.z};
    };

    Point3d& operator+=(const Point3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    };

    Point3d& operator-=(const Point3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    };

    Point3d& operator*=(const double scale) {
        x *= scale; y *= scale; z *= scale;
        return *this;
    };

    Point3d operator*(const double scale) const {
        return {x*scale, y*scale, z*scale};
    };
};

struct Vector3d
{
    Point3d origin;
    Point3d end;
};

double drand();
Point3d rnd_Point3d_direction();

double len_squared(Point3d vec);

sf::Color depth_shading(float depth_0to1, const sf::Color& color);

float perspective_multiplier(double z);
void display_point(sf::RenderWindow& window, sf::Vector2f window_center, Point3d coords, float RadiusInPixels, sf::Color color);
void display_line(sf::RenderWindow& window, sf::Vector2f window_center, Point3d c1, Point3d c2, sf::Color col1, sf::Color col2);

#endif