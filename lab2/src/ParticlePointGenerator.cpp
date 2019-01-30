#include "ParticlePointGenerator.hpp"
#include <glm/gtx/rotate_vector.hpp> 

ParticlePointGenerator::ParticlePointGenerator(float timeToLive, glm::vec3 _position, glm::vec3 _direction, float _angle, float _speed, ParticleTimeDistribution& timeDistribution)
    : SimpleParticleGenerator(timeToLive, timeDistribution), position(_position), direction(glm::normalize(_direction)), angle(_angle), speed(_speed) { }


std::vector<std::pair<Particle, double>> ParticlePointGenerator::generate(double deltaTime) {
    int num = timeDistribution.update(deltaTime);

    std::vector<std::pair<Particle, double>> genParts;
    for(int i = 0; i < num; i++){
        Particle particle;
        particle.position =  position;
        
        glm::vec3 temp = direction * 2.0f - glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 tx = glm::cross(direction, temp);
        glm::vec3 ty = glm::cross(direction, tx);

        particle.velocity = glm::rotate(direction, (float)distribution(generator) * angle, tx);
        particle.velocity = glm::rotate(particle.velocity, (float)distribution(generator) * angle, ty);
        particle.live = 1.0f;

        genParts.push_back(std::make_pair(particle, timeToLive));
    }
    return genParts;

}

void ParticlePointGenerator::setPosition(glm::vec3 _position) {
    position = _position;
}

void ParticlePointGenerator::setDirection(glm::vec3 _direction) {
    direction = _direction;
}
