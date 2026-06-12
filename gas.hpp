#ifndef _GRAPH_COMPONENTS_
#define _GRAPH_COMPONENTS_

#include "tools.hpp"
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;

#include <iostream>
#include <SFML/Graphics.hpp>

class Gas;
class Particle;

class Gas
{
private:
    list<Particle> particles;

    float yaw;
    float pitch;
public:
    int mode;

    Gas();
    ~Gas();

    Particle* emplace_particle(Vector3d coords, Vector3d velocity);
    bool erase_particle(Particle* particle);

    vector<Particle*> getParticles();
    
    // V3 and SFML

    void tick();
    void display(sf::RenderWindow& window);

    Vector3d calc_window_coords(Vector3d coords, float scale);

    void addYaw(float dyaw) { yaw += dyaw; };
    void addPitch(float dpitch) { pitch += dpitch; };
    void resetYawPitch() { yaw = pitch = 0; };
    
    // void display_grid(sf::RenderWindow& window, sf::Color grid_color);
    void display_xyz_axes(sf::RenderWindow& window, float scale);
};

class Particle
{
private:
    Vector3d coords;
    Vector3d velocity;
public:
    Particle(Vector3d coords, Vector3d velocity);
    ~Particle();

    // V3 and SFML
    
    Vector3d getCoords() const { return coords; };
    Vector3d getVelocity() const { return velocity; };

    void set_coords(Vector3d crd) { coords = crd; };
    void add_coords(Vector3d dcrd) { coords += dcrd; };

    void set_velocity(Vector3d vel) { velocity = vel; };
    void add_velocity(Vector3d dvel) { velocity += dvel; };

    void update_coords();
};

#endif
