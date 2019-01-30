#ifndef PARTICLE_MODEL_GENERATOR_HPP
#define PARTICLE_MODEL_GENERATOR_HPP

#include "SimpleParticleGenerator.hpp"

class ParticlePointGenerator : public SimpleParticleGenerator {
    private:
        glm::vec3 position;
        glm::vec3 direction;
        float angle;
        float speed;

    public:
        ParticlePointGenerator(glm::vec3 position, glm::vec3 direction, float angle, float speed, ParticleTimeDistributio timeDistribution);
        std::vector<std::pair<Particle, double>> generate(double deltaTime); 
        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction);

}
#endif
