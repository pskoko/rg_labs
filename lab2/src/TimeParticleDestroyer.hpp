#ifndef TIME_PARTICLE_DESTROYER_HPP
#define TIME_PARTICLE_DESTROYER_HPP

#include "ParticleDestroyer.hpp"
#include <queue>
#include <vector>


class TimeParticleDestroyer : public ParticleDestroyer{
    private:
        static bool compare(const std::pair<int, double>& a, const std::pair<int,double>& b) {
            return (a.second > b.second) ;
        };

        std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, decltype(&compare)> queue;
        double time = 0;
    public:
        TimeParticleDestroyer();
        void addCreatedParticle(int id, Particle particle, double timeToLive);
        std::vector<int> destroy(double deltaTime);
        ~TimeParticleDestroyer() = default;
};
#endif
