#ifndef _PARCER_
#define _PARCER_

class Config_keeper
{
public:
    static inline unsigned TPS = 20;
    static inline double dt = 1;
    static inline unsigned HOW_MANY_PARTICLES = 0;

    static inline double k_B = 1;
    static inline double PARTICLE_MASS = 1;

    Config_keeper() { };
    ~Config_keeper() { };

    void read();
};

#endif