#include <iostream>
using std::cin, std::cout, std::endl;

#include <cmath>
#include <string>
#include <sstream>

#include "tools.hpp"
#include "model.hpp"
#include "walls.hpp"
#include "defs.hpp"

int main() {

    // ====================== INITIALISATION OR SMTH ======================

    cout << "INITIALISATION OR SMTH" << endl;

    Model model;
    for ( int i = 0; i < 256; i++ )
        model.emplace_particle( Point3d({0, 0, 0}), rnd_Point3d_direction() * 2500 );

    model.emplace_rect_wall( {0.1, 0, 0}, (orthogonal_axis)0, 0.2, -1 );
    model.emplace_rect_wall( {-0.1, 0, 0}, (orthogonal_axis)0, 0.2, 1 );

    model.emplace_rect_wall( {0, 0.1, 0}, (orthogonal_axis)1, 0.2, -1 );
    model.emplace_rect_wall( {0, -0.1, 0}, (orthogonal_axis)1, 0.2, 1 );

    model.emplace_rect_wall( {0, 0, 0.1}, (orthogonal_axis)2, 0.2, -1 );
    model.emplace_rect_wall( {0, 0, -0.1}, (orthogonal_axis)2, 0.2, 1 );

    // ============================== WINDOW ==============================
    
    bool do_ticks = false;
    const int FPS = 60;
    const double delta_angle = 0.05;
    const sf::Time MSPT = sf::milliseconds(1000.f / TPS);
    
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