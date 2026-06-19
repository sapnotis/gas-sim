#ifndef _PARCER_
#define _PARCER_

#include <string>
using std::string;

class Txt_handler
{
private:
    static inline int log_id = 0;
    
public:
    static inline unsigned TPS = 20;
    static inline double dt = 1;
    static inline unsigned HOW_MANY_PARTICLES = 0;

    static inline double wall_velocity = 0;

    static inline double k_B = 1;
    static inline double PARTICLE_MASS = 1;

    Txt_handler() { };
    ~Txt_handler() { };

    void read_config();
    static void log(string str);
};

#endif