#ifndef _MODEL_LOGIC_
#define _MODEL_LOGIC_

#include "tools.hpp"
#include "walls.hpp"

#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;

#include <iostream>
#include <SFML/Graphics.hpp>

class Model;
class Particle;

class Model
{
private:
    list<Particle> particles;
    vector<RectangularWall> rect_walls; // no polymorphism today
    float yaw;
    float pitch;
    float scale;

public:
    Model() : yaw(0), pitch(0), scale(14) { };
    ~Model() { };

    Particle* emplace_particle(Point3d coords, Point3d velocity);
    bool erase_particle(Particle* particle);

    RectangularWall* emplace_rect_wall(Point3d midpoint, orthogonal_axis axis, double radius, double velocity);
    bool erase_rect_wall(RectangularWall* wall);

    vector<Particle*> getParticles();
    
    // V3 and SFML

    void tick();
    void display(sf::RenderWindow& window);

    Point3d calc_window_coords(Point3d coords);

    void addYaw(float dyaw) { yaw += dyaw; };
    void addPitch(float dpitch) { pitch += dpitch; };
    void resetYawPitch() { yaw = pitch = 0; };
    
    void display_xyz_axes(sf::RenderWindow& window);
};

class Particle
{
private:
    Point3d coords;
    Point3d velocity;
public:
    Particle(Point3d coords, Point3d velocity);
    ~Particle();

    // V3 and SFML
    
    Point3d getCoords() const { return coords; };
    Point3d getVelocity() const { return velocity; };

    void set_coords(Point3d crd) { coords = crd; };
    void add_coords(Point3d dcrd) { coords += dcrd; };

    void set_velocity(Point3d vel) { velocity = vel; };
    void add_velocity(Point3d dvel) { velocity += dvel; };

    void update_coords(const vector<RectangularWall>& walls);
};

#endif