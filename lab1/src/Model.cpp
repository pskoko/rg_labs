#include "Model.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Model::Model(const char* filename_) : filename(filename_) {}
void Model::loadModel(){
    std::ifstream modelFile(filename);
    
    std::string line;
    while(getline(modelFile, line)){
       if(line[0] == 'v'){
            std::istringstream iss( line );
            std::string v;
            Vertex vertex;
            iss >> v >> vertex.position[0] >> vertex.position[1] >> vertex.position[2];  
            vertices.push_back(vertex);
        } else if (line[0] == 'f'){
            std::istringstream iss( line );
            std::string v;
            unsigned int a, b, c;
            iss >> v >> a >> b >> c;  
            faces.push_back(a-1);
            faces.push_back(b-1);
            faces.push_back(c-1);
        }
       
    }
    std::vector<int> nums(vertices.size(), 0);
    for(int i = 0; i < faces.size(); i+=3){
        glm::vec3 a = vertices.at(faces.at(i)).position;
        glm::vec3 b = vertices.at(faces.at(i+1)).position;
        glm::vec3 c = vertices.at(faces.at(i+2)).position;
        glm::vec3 normal = glm::triangleNormal(a, b, c);
        

        vertices.at(faces.at(i)).normal += normal;
        vertices.at(faces.at(i+1)).normal += normal;
        vertices.at(faces.at(i+2)).normal += normal;
        nums.at(faces.at(i)) += 1;
        nums.at(faces.at(i+1)) += 1;
        nums.at(faces.at(i+2)) += 1;
    }

    glm::vec3 mean(0.0f, 0.0f, 0.0f); 
    for(int i = 0; i < vertices.size(); i++){
        vertices.at(i).normal *= static_cast<float>(1.0/nums.at(i));
        mean += vertices.at(i).position;
    }

    /*
    mean /= static_cast<float>(vertices.size());
    for(int i = 0; i < vertices.size(); i++){
        vertices.at(i).position -= mean;
    }
    */

    pShader = new Shader("src/shaders/model.vert", "src/shaders/model.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), faces.data(), GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Model::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, glm::vec3 lightPos){
    glBindVertexArray(VAO);
    pShader->use();
    pShader->setVec3("objectColor", 0.5f, 0.5f, 0.31f);
    pShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    pShader->setVec3("lightPos", lightPos);
    pShader->setVec3("viewPos", cameraPos);
    pShader->setMat4("model", model);
    pShader->setMat4("view", view);
    pShader->setMat4("projection", projection);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
