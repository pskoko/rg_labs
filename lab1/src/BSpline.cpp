#include "BSpline.hpp"
#include <iostream>
#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

BSpline::BSpline(std::vector<glm::vec3> &controlPoints_) : controlPoints(controlPoints_)
{
}

void BSpline::init(){
    generateVertices();
    pShader = new Shader("src/shaders/bspline.vert", "src/shaders/bspline.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, generatedVertices.size()*sizeof(glm::vec4), generatedVertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void BSpline::generateSegment(unsigned int numSegment)
{
    glm::mat4x3 positions;

    for (int i = 0; i < 4; i++)
    {
        positions[i] = controlPoints.at(numSegment - 1 + i);
    }

    for (unsigned int i = 0; i <= verticesPerSegment; i++)
    {
        float param = i / static_cast<float>(verticesPerSegment);
        glm::vec4 paramVec(param * param * param, param * param, param, 1.0f);
        //std::cout << glm::to_string(paramVec) << std::endl;
        //std::cout << glm::to_string(glm::transpose(genMatrix)) << std::endl;
        generatedVertices.push_back(glm::vec4(paramVec * static_cast<float>(1. / 6) * glm::transpose(genMatrix)* glm::transpose(positions), 1.0));
    }
}

void BSpline::generateVertices()
{
    for (unsigned int i = 0; i < controlPoints.size() - 3; i++)
    {
        generateSegment(i + 1);
    }

    glm::vec4 start = glm::vec4(getPosition(position), 0.f);
    glm::vec4 end = start + 1.0f*glm::normalize(glm::vec4(getDerivation(position), 0.f));
    std::cout << glm::to_string(start) << std::endl;
    generatedVertices.push_back(start);
    generatedVertices.push_back(end);
    
    /*
    for(int i = 0; i < generatedVertices.size(); i++){
        std::cout << glm::to_string(generatedVertices.at(i)) << std::endl;
    }
    */

}

void BSpline::updateParameter(float deltaTime){
    position += deltaTime * 1.0f;
    position = std::max(position, 0.f);
    position = std::min(position, static_cast<float>(controlPoints.size() - 3)-0.001f);
    glm::vec4 start = glm::vec4(getPosition(position), 0.f);
    glm::vec4 end = start + 1.0f*glm::normalize(glm::vec4(getDerivation(position), 0.f));
    generatedVertices.at(generatedVertices.size()-2) = start;
    generatedVertices.at(generatedVertices.size()-1) = end;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, (generatedVertices.size()-2)*sizeof(glm::vec4), 2*sizeof(glm::vec4), generatedVertices.data() + generatedVertices.size()-2);
    
}
void BSpline::draw(glm::mat4 vp)
{
    glBindVertexArray(VAO);
    pShader->use();
    pShader->setMat4("vp", vp);
    glDrawArrays(GL_LINE_STRIP, 0, generatedVertices.size()-2);
    glDrawArrays(GL_LINES, generatedVertices.size()-2, 2);
    glBindVertexArray(0);
}

void BSpline::debug(glm::mat4 vp)
{
    std::cout << "Debug" << std::endl;
    for (int i = 0; i < controlPoints.size(); i++)
    {
        glm::vec4 temp = glm::vec4(controlPoints.at(i), 1.0);
        std::cout << glm::to_string(controlPoints.at(i)) << " after " << glm::to_string(vp * temp) << std::endl;
    }
}

glm::vec3 BSpline::getCurrentPosition(){
    return getPosition(position);
}

glm::vec3 BSpline::getCurrentDerivation(){
    return getDerivation(position);
}

glm::vec3 BSpline::getDerivation(float param){
    int numSeg = static_cast<int>(param);
    param -= numSeg;

    glm::mat4x3 positions;
    for (int i = 0; i < 4; i++){
        positions[i] = controlPoints.at(numSeg + i);
    }

    glm::vec3 params(param*param, param, 1);
    glm::vec3 result = static_cast<float>(1./2) * params * glm::transpose(devMatrix) * glm::transpose(positions);
    return result;

}

glm::vec3 BSpline::getPosition(float param){
    int numSeg = static_cast<int>(param);
    param -= numSeg;

    glm::mat4x3 positions;
    for (int i = 0; i < 4; i++){
        positions[i] = controlPoints.at(numSeg + i);
    }

    glm::vec4 params(param*param*param, param*param, param, 1);
    glm::vec3 result = params* static_cast<float>(1. / 6) * glm::transpose(genMatrix)* glm::transpose(positions);
    
    return result;

}

void BSpline::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
}

BSpline::~BSpline()
{
    cleanup();
    delete pShader;
}
