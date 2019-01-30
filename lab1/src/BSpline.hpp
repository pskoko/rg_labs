#ifndef BSPLINE_HPP
#define BSPLINE_HPP

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"

class BSpline {   
private:
    std::vector<glm::vec3> controlPoints;
    std::vector<glm::vec4> generatedVertices;
    unsigned int verticesPerSegment = 100;
    unsigned int VAO;
    unsigned int VBO;
    Shader* pShader;

    float position = 0.0f;
    void generateSegment(unsigned int numSegment);
    void generateVertices();

    glm::mat4 genMatrix = glm::mat4(-1.0f, 3.0f, -3.0f, 1.0f,
                                          3.0f, -6.0f, 3.0f, 0.0f,
                          -3.0f, 0.0f, 3.0f, 0.0f,
                           1.0f, 4.0f, 1.0f, 0.0f);
    glm::mat3x4 devMatrix = glm::mat3x4(-1.0f, 3.0f, -3.0f, 1.0f,
                                        2.0f, -4.0f, 2.0f, 0.0f,
                                        -1.0f, 0.0f, 1.0f, 0.0f);
    void debug(glm::mat4);
public:
    BSpline(std::vector<glm::vec3>& controlPoints_);
    void init();
    glm::vec3 getCurrentDerivation();
    glm::vec3 getCurrentPosition();
    glm::vec3 getDerivation(float param);
    glm::vec3 getPosition(float param);
    void updateParameter(float deltaTime);
    void draw(glm::mat4 vp);
    void cleanup();
    ~BSpline();

};


#endif 
