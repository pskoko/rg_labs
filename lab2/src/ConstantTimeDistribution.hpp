#ifndef CONSTANT_TIME_DISTRIBUTION
#define CONSTANT_TIME_DISTRIBUTION

#include "ParticleTimeDistribution.hpp"

class ConstantTimeDistribution : public ParticleTimeDistribution {
    protected:
        float particlesPerSecond;
    public:
        ConstantTimeDistribution(double particlesPerSecond);
        int update(float deltaTime);
};


#endif
