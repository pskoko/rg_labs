#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Particle{

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    float time = 0.0f;
    float live = -1.0f;
};

#endif
