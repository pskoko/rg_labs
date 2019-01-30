#ifndef PARTICLE_QUAD_GENERATOR_HPP
#define PARTICLE_QUAD_GENERATOR_HPP

#include <random>

#include "SimpleParticleGenerator.hpp"

class ParticleQuadGenerator : public SimpleParticleGenerator {
    private:
        glm::vec3 speed;
        glm::vec3 center;
        glm::vec3 rightSide;
        glm::vec3 leftSide;
        double prev = 0;

    public:
        ParticleQuadGenerator( double _timeToLive, glm::vec3 _speed, glm::vec3 _center, glm::vec3 _rightSide, glm::vec3 _leftSide, ParticleTimeDistribution& timeDistribution);
        std::vector<std::pair<Particle, double>> generate(double deltaTime);
        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction);
        virtual ~ParticleQuadGenerator() = default;
};

#endif
