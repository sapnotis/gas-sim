#include "config_keeper.hpp"

#include <fstream>
#include <sstream>

#include <iostream>
using std::cout, std::endl;

Config_keeper::Config_keeper() {

    std::ifstream config("config.txt");
    if (!config.is_open()) {
        cout << "Couldn't find config! exit" << endl;
        return;
    }

    std::string line;

    std::getline(config, line);
    std::stringstream ss(line);
    ss >> Config_keeper::TPS;

    std::getline(config, line);
    std::stringstream ss(line);
    ss >> Config_keeper::dt;

    std::getline(config, line);
    std::stringstream ss(line);
    ss >> Config_keeper::HOW_MANY_PARTICLES;

    std::getline(config, line);
    std::stringstream ss(line);
    ss >> Config_keeper::k_B;

    std::getline(config, line);
    std::stringstream ss(line);
    ss >> Config_keeper::PARTICLE_MASS;

    // while (std::getline(config, line)) {

    //     if ( line[0]=='#' || line.size()==1 )
    //         continue;
        
    //     ;
    //     ss >> ...
    // }

    config.close();
}