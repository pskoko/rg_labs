#ifndef PARTICLE_DESTROYER_HPP
#define PARTICLE_DESTROYER_HPP

#include "Particle.hpp"
#include <vector>

class ParticleDestroyer {
    public:
        virtual void addCreatedParticle(int id, Particle particle, double timeToLive) = 0;
        virtual std::vector<int> destroy(double deltaTime) = 0;
        virtual ~ParticleDestroyer() = default;
};
#endif
