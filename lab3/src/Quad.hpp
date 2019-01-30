//
// Created by pskoko on 1/23/19.
//

#ifndef PARTICLES_QUAD_HPP
#define PARTICLES_QUAD_HPP


#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/detail/type_mat.hpp>
#include <string>
#include "Shader.hpp"

class Quad {
private:
    glm::vec3 pos[4];
    glm::vec2 uv[4];
    glm::vec3 normal, tangent[2], bitangent[2];
    unsigned int VAO, VBO;
    Shader* pShader;
    unsigned int textureId, normalId, heightId;
    float heightScale = 0.1;
    unsigned int numLayers = 5;
    int type = 0;
public:
    Quad(glm::vec3 position, glm::vec3 side1, glm::vec3 side2, std::string texture, std::string bumpMap, std::string heightMap);
    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, glm::vec3 lightPos);
    void setType(unsigned int type);
    void incrementScale(float increment);
    void incrementNumLayers(int increment);
};


#endif //PARTICLES_QUAD_HPP
