#include "ParticleSystem.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <iostream>
#include <stb_image.h>

void ParticleSystem::updateState(double deltaTime){
    prevTime += deltaTime;
    auto destroyedIds = particleDestroyer.destroy(deltaTime);
    for(int id: destroyedIds){
        particles[id].live = -1.0f;
        deadParticles.push(id);
        toUpdateParticles.insert(id);
    }

    auto createdParticles = particleGenerator.generate(deltaTime);
    for(auto particlePair: createdParticles){
        particlePair.first.time = prevTime;
        if(deadParticles.empty()) break;

        int id = deadParticles.front();
        deadParticles.pop();
        particles[id] = particlePair.first;
        particleDestroyer.addCreatedParticle(id, particlePair.first, particlePair.second);
        toUpdateParticles.insert(id);
    }
}

void ParticleSystem::initDraw(){
    pShader = new Shader("src/shaders/particle.vert", "src/shaders/particle.frag", "src/shaders/particle.gs");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFilename.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void ParticleSystem::draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection, float ratio){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    updateBuffer();
    pShader->use();
    
    pShader->setVec3("viewPos", cameraPos);
    pShader->setMat4("view", view);
    pShader->setMat4("projection", projection);
    pShader->setFloat("time", prevTime);
    pShader->setFloat("ratio", ratio);
    pShader->setFloat("size", size);
    glDrawArrays(GL_POINTS, 0, particles.size());
    glBindVertexArray(0);

}

ParticleSystem::ParticleSystem(ParticleGenerator& _particleGenerator, ParticleDestroyer& _particleDestroyer, int _maxParticles, std::string _textureFilename, float _size)
    : particleGenerator(_particleGenerator), particleDestroyer(_particleDestroyer), maxParticles(_maxParticles), textureFilename(_textureFilename), size(_size) {
        for(int i = 0; i < _maxParticles; i++){
            deadParticles.push(i);
            toUpdateParticles.insert(i);
        }

        particles = std::vector<Particle> (maxParticles, Particle());
    }

void ParticleSystem::updateBuffer(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    Particle* particleBuffer = static_cast<Particle*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

    for(int id: toUpdateParticles){
        std::memcpy(particleBuffer + id, &particles[id], sizeof(Particle));
    }
    
    toUpdateParticles.clear();
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
