#include "PeriodicTimeDistribution.hpp"
#include <cmath>


PeriodicTimeDistribution::PeriodicTimeDistribution(float _minPerSec, float _maxPerSec, float _period)
    : minPerSec(_minPerSec), maxPerSec(_maxPerSec), period(_period) {}

int PeriodicTimeDistribution::update(float deltaTime){
    float sum = ((maxPerSec - minPerSec)*(std::sin(M_PI/period * time) +  std::sin(M_PI/period * (time+deltaTime)) + 2) + 2 * minPerSec)/2 * deltaTime ;
    time += deltaTime;
    
    prev += sum;
    int num = std::ceil(prev);
    prev -= num;

    return num;
}
