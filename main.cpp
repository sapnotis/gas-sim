#include <iostream>
using std::cin, std::cout, std::endl;

#include <cmath>
#include <string>
#include <sstream>

#include "tools.hpp"
#include "model.hpp"
#include "walls.hpp"
#include "config_keeper.hpp"
#include "style_keeper.hpp"

int main() {

    // ====================== INITIALISATION OR SMTH ======================

    cout << "INITIALISATION OR SMTH" << endl;

    Model model(1000);

    double wall_velocity = 0.02;
    double wall_size = 0.2;

    model.emplace_rect_wall( {wall_size, 0, 0}, (orthogonal_axis)0, wall_size, -wall_velocity );
    model.emplace_rect_wall( {-wall_size, 0, 0}, (orthogonal_axis)0, wall_size, 0 );

    model.emplace_rect_wall( {0, wall_size, 0}, (orthogonal_axis)1, wall_size, 0 );
    model.emplace_rect_wall( {0, -wall_size, 0}, (orthogonal_axis)1, wall_size, 0 );

    model.emplace_rect_wall( {0, 0, wall_size}, (orthogonal_axis)2, wall_size, 0 );
    model.emplace_rect_wall( {0, 0, -wall_size}, (orthogonal_axis)2, wall_size, 0 );

    for ( unsigned int i = 0; i < Config_keeper::HOW_MANY_PARTICLES; i++ ) {
        model.emplace_particle( Point3d({0, 0, 0}), maxwell_3d_distribution(0.05) );
    }

    // ============================== WINDOW ==============================
    
    bool do_ticks = false;
    const int FPS = 60;
    const double delta_angle = 0.05;
    const sf::Time MSPT = sf::milliseconds(1000.f / Config_keeper::TPS);
    
    sf::RenderWindow window( sf::VideoMode( 1600, 900 ), "Gas" );
    window.setFramerateLimit( FPS );
    
    sf::Clock clock;
    sf::Time since_last_tick = sf::Time::Zero;

    while (window.isOpen()) {

        sf::Time deltat = clock.restart();
        since_last_tick += deltat;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

                if (event.key.code == sf::Keyboard::R)
                    model.resetYawPitch();
                
                if (event.key.code == sf::Keyboard::Space)
                    do_ticks = !do_ticks;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            model.addYaw( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            model.addYaw( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            model.addPitch( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            model.addPitch( delta_angle );

        while (since_last_tick > MSPT) {

            since_last_tick -= MSPT;
            if ( do_ticks )
                model.tick();
        }

        window.clear(bc_color);

        model.display(window);
        
        window.display();
    }

    return 0;
}