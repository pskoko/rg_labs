#include "ParticleQuadGenerator.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ParticleQuadGenerator::ParticleQuadGenerator(double _timeToLive, glm::vec3 _speed, glm::vec3 _center, glm::vec3 _rightSide, glm::vec3 _leftSide, ParticleTimeDistribution& timeDistribution) 
    : SimpleParticleGenerator(_timeToLive, timeDistribution),  speed(_speed),  center(_center), rightSide(_rightSide), leftSide(_leftSide) { 
    };

std::vector<std::pair<Particle, double>> ParticleQuadGenerator::generate(double deltaTime){ 
    int num = timeDistribution.update((float)deltaTime);

    std::vector<std::pair<Particle, double>> genParts;
    for(int i = 0; i < num; i++){
        Particle particle;
        particle.position =  rightSide * (float)distribution(generator) + leftSide * (float)distribution(generator)+ center;
        particle.velocity = speed;
        particle.live = 1.0f;

        genParts.push_back(std::make_pair(particle, timeToLive));
    }
    return genParts;
}

void ParticleQuadGenerator::setPosition(glm::vec3 position){

}

void ParticleQuadGenerator::setDirection(glm::vec3 direction){
}
