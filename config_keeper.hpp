#ifndef _PARCER_
#define _PARCER_

class Config_keeper
{
public:
    static inline unsigned TPS = 20;
    static inline double dt = 1;
    static inline unsigned HOW_MANY_PARTICLES = 0;

    static inline double k_B = 1.380649e-23;
    static inline double PARTICLE_MASS = 1.673e-27;

    Config_keeper();
};

#endif