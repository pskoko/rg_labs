//
// Created by pskoko on 1/23/19.
//

#include <glm/detail/type_mat4x4.hpp>
#include <glad/glad.h>
#include "Quad.hpp"
#include "stb_image.h"
#include <iostream>

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Quad::Quad(glm::vec3 position, glm::vec3 side1, glm::vec3 side2, std::string texture, std::string bumpMap, std::string heightMap) {
    pos[0] = position + side2;
    pos[1] = position;
    pos[2] = position + side1;
    pos[3] = position + side1 + side2;

    uv[0] = glm::vec2(0.0f, 1.0f);
    uv[1] = glm::vec2(0.0f, 0.0f);
    uv[2] = glm::vec2(1.0f, 0.0f);
    uv[3] = glm::vec2(1.0f, 1.0f);

    normal = glm::cross(side1, side2);

    glm::vec3 edge1 = pos[1] - pos[0];
    glm::vec3 edge2 = pos[2] - pos[0];
    glm::vec2 deltaUV1 = uv[1] - uv[0];
    glm::vec2 deltaUV2 = uv[2] - uv[0];

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent[0].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent[0].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent[0].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent[0] = glm::normalize(tangent[0]);

    bitangent[0].x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent[0].y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent[0].z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent[0] = glm::normalize(bitangent[0]);

    edge1 = pos[2] - pos[0];
    edge2 = pos[3] - pos[0];
    deltaUV1 = uv[2] - uv[0];
    deltaUV2 = uv[3] - uv[0];

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent[1].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent[1].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent[1].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent[1] = glm::normalize(tangent[1]);


    bitangent[1].x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent[1].y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent[1].z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent[1] = glm::normalize(bitangent[1]);

    float quadVertices[] = {
            pos[0].x, pos[0].y, pos[0].z, normal.x, normal.y, normal.z, uv[0].x, uv[0].y, tangent[0].x, tangent[0].y, tangent[0].z, bitangent[0].x, bitangent[0].y, bitangent[0].z,
            pos[1].x, pos[1].y, pos[1].z, normal.x, normal.y, normal.z, uv[1].x, uv[1].y, tangent[0].x, tangent[0].y, tangent[0].z, bitangent[0].x, bitangent[0].y, bitangent[0].z,
            pos[2].x, pos[2].y, pos[2].z, normal.x, normal.y, normal.z, uv[2].x, uv[2].y, tangent[0].x, tangent[0].y, tangent[0].z, bitangent[0].x, bitangent[0].y, bitangent[0].z,

            pos[0].x, pos[0].y, pos[0].z, normal.x, normal.y, normal.z, uv[0].x, uv[0].y, tangent[1].x, tangent[1].y, tangent[1].z, bitangent[1].x, bitangent[1].y, bitangent[1].z,
            pos[2].x, pos[2].y, pos[2].z, normal.x, normal.y, normal.z, uv[2].x, uv[2].y, tangent[1].x, tangent[1].y, tangent[1].z, bitangent[1].x, bitangent[1].y, bitangent[1].z,
            pos[3].x, pos[3].y, pos[3].z, normal.x, normal.y, normal.z, uv[3].x, uv[3].y, tangent[1].x, tangent[1].y, tangent[1].z, bitangent[1].x, bitangent[1].y, bitangent[1].z
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glBindVertexArray(0);

    pShader = new Shader("src/shaders/quad_para.vert", "src/shaders/quad_para.frag");
    textureId = loadTexture(texture.c_str());
    normalId = loadTexture(bumpMap.c_str());
    heightId = loadTexture(heightMap.c_str());

    pShader->use();
    pShader->setInt("diffuseMap", 0);
    pShader->setInt("normalMap", 1);
    pShader->setInt("depthMap", 2);
    pShader->setFloat("heightScale", 0.1f);
    pShader->setInt("type", 0);
    pShader->setInt("numLayers", 5);
}

void Quad::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, glm::vec3 lightPos) {
    glBindVertexArray(VAO);
    pShader->use();
    pShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    pShader->setVec3("lightPos", lightPos);
    pShader->setVec3("viewPos", cameraPos);
    pShader->setMat4("model", model);
    pShader->setMat4("view", view);
    pShader->setMat4("projection", projection);
    pShader->setFloat("heightScale", heightScale);
    pShader->setInt("type", type);
    pShader->setInt("numLayers", numLayers);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalId);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, heightId);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Quad::setType(unsigned int type) {
    this->type = type;
}

void Quad::incrementScale(float increment) {
    heightScale += increment;
}

void Quad::incrementNumLayers(int increment) {
    if(numLayers == 1 && increment < 0) return;
    numLayers += increment;
}
