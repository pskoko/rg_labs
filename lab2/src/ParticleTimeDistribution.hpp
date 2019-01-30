#ifndef PARTICLE_TIME_DISTRIBUTION_HPP
#define PARTICLE_TIME_DISTRIBUTION_HPP

class ParticleTimeDistribution {
    protected:
        float time = 0;
    public:
        virtual int update(float deltaTime) = 0;
        virtual ~ParticleTimeDistribution() = default;
};

#endif
