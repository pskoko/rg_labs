#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "Particle.hpp"
#include "ParticleGenerator.hpp"
#include "ParticleDestroyer.hpp"
#include "Shader.hpp"
#include <queue>
#include <set>
#include <vector>
#include <string>

class ParticleSystem{
    private:
        ParticleGenerator& particleGenerator;
        ParticleDestroyer& particleDestroyer;
        const int maxParticles;
        unsigned int VAO;
        unsigned int VBO;
        Shader* pShader;
        double prevTime = 0;
        void updateBuffer();
        std::vector<Particle> particles;
        std::set<int> toUpdateParticles;
        std::queue<int> deadParticles;
        std::string textureFilename;
        float size = 1.0;
        unsigned int texture;

    public:
        ParticleSystem(ParticleGenerator& particleGenerator, ParticleDestroyer& particleDestroyer, int maxParticles, std::string textureFilename, float size = 1.0f);
        void updateState(double deltaTime);
        void initDraw();
        void draw(glm::vec3 camerPos, glm::mat4 view, glm::mat4 projection, float ratio);

};
#endif
