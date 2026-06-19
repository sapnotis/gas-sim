#include "config_keeper.hpp"

#include <fstream>
#include <sstream>

#include <iostream>
using std::cout, std::endl;

void Config_keeper::read() {

    std::ifstream config("io/config.txt");
    if (!config.is_open()) {
        cout << "Couldn't find io/config! exit" << endl;
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
            ss >> Config_keeper::TPS;
            break;
        case 1:
            ss >> Config_keeper::dt;
            break;
        case 2:
            ss >> Config_keeper::HOW_MANY_PARTICLES;
            break;
        case 3:
            ss >> Config_keeper::k_B;
            break;
        case 4:
            ss >> Config_keeper::PARTICLE_MASS;
            break;
        default: {
            cout << "Too many config values" << endl;
            exit(1);
        }
        }

        counter++;
    }

    cout << HOW_MANY_PARTICLES << endl;

    config.close();
}