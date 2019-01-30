#ifndef PERIODIC_TIME_DISTRIBUTION_HPP
#define PERIODIC_TIME_DISTRIBUTION_HPP

#include "ParticleTimeDistribution.hpp"

class PeriodicTimeDistribution : public ParticleTimeDistribution {
    private:
        float minPerSec;
        float maxPerSec;
        float period;
        float prev = 0.0f;
    public:
        PeriodicTimeDistribution(float minPerSec, float maxPerSec, float period);
        int update(float deltaTime);
};

#endif
