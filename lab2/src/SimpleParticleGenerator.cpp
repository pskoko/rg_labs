#include "SimpleParticleGenerator.hpp"

SimpleParticleGenerator::SimpleParticleGenerator(float _timeToLive, ParticleTimeDistribution& _timeDistribution) 
    : timeDistribution(_timeDistribution), timeToLive( _timeToLive) {}
