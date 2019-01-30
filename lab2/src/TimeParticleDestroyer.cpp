#include "TimeParticleDestroyer.hpp"
#include <iostream>

TimeParticleDestroyer::TimeParticleDestroyer() : queue(&compare) {}

void TimeParticleDestroyer::addCreatedParticle(int id, Particle particle, double timeToLive){
    if(timeToLive == -1) return;

    queue.push(std::make_pair(id, timeToLive+time));
}

/*
static bool TimeParticledestroyer::compare(const std::pair<int, double>& a, const std::pair<int,double>& b){
    return (a.second < b.second) || (a.second == b.second && a.first < b.first);
}
*/

std::vector<int> TimeParticleDestroyer::destroy(double deltaTime){
    time += deltaTime;
    
    std::vector<int> destroyed;
    while(!queue.empty() && queue.top().second <= time){
        destroyed.push_back(queue.top().first); 
        queue.pop();
    }
    return destroyed;
}
