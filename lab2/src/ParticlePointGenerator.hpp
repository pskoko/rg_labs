#ifndef PARTICLE_POINT_GENERATOR
#define PARTICLE_POINT_GENERATOR

#include "SimpleParticleGenerator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ParticlePointGenerator : public SimpleParticleGenerator {
    private:
        glm::vec3 position;
        glm::vec3 direction;
        float angle;
        float speed;
    public:
        ParticlePointGenerator(float timeToLive, glm::vec3 position, glm::vec3 direction, float angle, float speed, ParticleTimeDistribution& timeDistribution);
        std::vector<std::pair<Particle, double>> generate(double deltaTime);
        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction);
};
#endif
