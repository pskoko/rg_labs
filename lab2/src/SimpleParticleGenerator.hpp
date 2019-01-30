#ifndef SIMPLE_PARTICLE_GENERATOR_HPP
#define SIMPLE_PARTICLE_GENERATOR_HPP

#include "ParticleGenerator.hpp"
#include "ParticleTimeDistribution.hpp"
#include <glm/glm.hpp>
#include <random>

class SimpleParticleGenerator : public ParticleGenerator {
    protected:
        ParticleTimeDistribution& timeDistribution;
        float timeToLive;
        std::default_random_engine generator;        
        std::uniform_real_distribution<double> distribution { std::uniform_real_distribution<double>(-0.5, 0.5)}; 

    public:
        SimpleParticleGenerator(float timeToLive, ParticleTimeDistribution& timeDistribution);
        virtual void setPosition(glm::vec3 position) = 0;
        virtual void setDirection(glm::vec3 direction) = 0;
        virtual ~SimpleParticleGenerator() = default;
};

#endif
