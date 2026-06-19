#include "txt_handler.hpp"

#include <fstream>
#include <sstream>

#include <iostream>
using std::cout, std::endl;

void Txt_handler::read_config() {
    
    std::ifstream config("io/config.txt");
    if (!config.is_open()) {
        cout << "(!) Couldn't find: io/config.txt!" << endl;
        return;
    }

    std::string line;
    unsigned counter = 0;

    while ( std::getline(config, line) ) {

        if ( line[0]=='#' || line.size()==1 )
            continue;
        
        std::stringstream ss(line);

        switch (counter)
        {
        case 0:
            ss >> Txt_handler::log_id;
            break;
        case 1:
            ss >> Txt_handler::TPS;
            break;
        case 2:
            ss >> Txt_handler::dt;
            break;
        case 3:
            ss >> Txt_handler::HOW_MANY_PARTICLES;
            break;
        case 4:
            ss >> Txt_handler::wall_velocity;
            break;
        case 5:
            ss >> Txt_handler::k_B;
            break;
        case 6:
            ss >> Txt_handler::PARTICLE_MASS;
            break;
        default: {
            cout << "(!) Smth wrong with config values" << endl;
            config.close();
            exit(1);
        }
        }

        counter++;
    }

    Txt_handler::dt = 0.5 / Txt_handler::wall_velocity / 200;

    cout << "+ Config read successfully" << endl;

    std::ofstream log("io/log_" + std::to_string(log_id) + ".txt");
    log << "Wall vel: " << Txt_handler::wall_velocity << endl;

    if (!log.is_open()) {
        cout << "(!) Couldn't log to: io/log_" + std::to_string(log_id) + ".txt!" << endl;
        return;
    }

    log.close();

    config.close();
}

void Txt_handler::log(std::string str) {

    std::ofstream log("io/log_" + std::to_string(log_id) + ".txt", std::ios::app);

    if (!log.is_open()) {
        cout << "(!) Couldn't log to: io/log_" + std::to_string(log_id) + ".txt!" << endl;
        return;
    }

    log << str << endl;
    log.close();
}