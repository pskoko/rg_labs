#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include "Particle.hpp"
#include <utility>
#include <vector>

class ParticleGenerator{
    public:
        virtual std::vector<std::pair<Particle, double>> generate(double time) = 0;
        virtual ~ParticleGenerator() = default;
    
};
#endif

