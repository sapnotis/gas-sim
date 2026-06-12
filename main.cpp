#include <iostream>
using std::cin, std::cout, std::endl;
#include "gas.hpp"

#include <cmath>
#include <string>
#include <sstream>

int main() {

    // ====================== INITIALISATION OR SMTH ======================

    cout << "INITIALISATION OR SMTH" << endl;

    Gas gas;
    for ( int i = 0; i < 1000; i++ )
        gas.emplace_particle(rnd_Vector3d_direction() * 10, rnd_Vector3d_direction());

    // ============================== WINDOW ==============================

    bool do_ticks = false;
    const int FPS = 60;
    const double delta_angle = 0.05;
    const sf::Time MSPT = sf::milliseconds(1000.f / 20.0f);
    
    sf::RenderWindow window( sf::VideoMode( 1600, 900 ), "Gas" );
    window.setFramerateLimit( FPS );
    
    sf::Clock clock;
    sf::Time since_last_tick = sf::Time::Zero;

    while (window.isOpen()) {

        sf::Time dt = clock.restart();
        since_last_tick += dt;

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
                    gas.resetYawPitch();
                
                if (event.key.code == sf::Keyboard::Space)
                    do_ticks = !do_ticks;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            gas.addYaw( delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            gas.addYaw( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            gas.addPitch( - delta_angle );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            gas.addPitch( delta_angle );

        while (since_last_tick > MSPT) {

            since_last_tick -= MSPT;
            if ( do_ticks )
                gas.tick();
        }

        window.clear(sf::Color(3, 16, 25));

        gas.display(window);
        
        window.display();
    }

    return 0;
}