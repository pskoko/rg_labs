#include "ConstantTimeDistribution.hpp"
#include <cmath>

ConstantTimeDistribution::ConstantTimeDistribution(double _particlesPerSecond) : particlesPerSecond(_particlesPerSecond) {

}
    
int ConstantTimeDistribution::update(float deltaTime) {
    time += deltaTime * particlesPerSecond;
    int num = std::floor(time);
    time -= num;

    return num;
}
