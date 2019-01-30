#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Shader.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
};

class Model{
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::string filename;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> faces;
    Shader* pShader;

public:
    Model(const char* filename);
    void loadModel();
    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, glm::vec3 lightPos);
};
