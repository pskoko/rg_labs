#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Shader.hpp>
#include <Camera.hpp>

#include "BSpline.hpp"
#include "Model.hpp"

#include "ParticleSystem.hpp"
#include "ParticleQuadGenerator.hpp"
#include "TimeParticleDestroyer.hpp"
#include "ParticlePointGenerator.hpp"
#include "ConstantTimeDistribution.hpp"
#include "PeriodicTimeDistribution.hpp"


#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::vector<glm::vec3> temp{glm::vec3(0, 0, 0), glm::vec3(0, 10, 5), glm::vec3(10, 10, 10), glm::vec3(10, 0, 15),
                            glm::vec3(0, 0, 20), glm::vec3(0, 10, 25), glm::vec3(10, 10, 30), glm::vec3(10, 0, 35), 
                            glm::vec3(0, 0, 40), glm::vec3(0, 10, 45), glm::vec3(10, 10, 50), glm::vec3(10, 0, 55),
                            glm::vec3(15, 15, 20), glm::vec3(80, 30, 25), glm::vec3(65, 43, 30)};
BSpline bspline(temp);

glm::vec3 lightPos(1.2f, 1.0f, 5.0f);
glm::vec3 start(0.0f, 0.0f, 1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particle system", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    bspline.init();
    Model model("resources/models/f16.obj");
    model.loadModel();

    ConstantTimeDistribution constantDistribution(500);
    ParticleQuadGenerator generator(20, glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 00.0f, 100.0f), constantDistribution);
    TimeParticleDestroyer destroyer, destroyerPoint, destroyerRain;

    ConstantTimeDistribution constantDistributionPoint(50);
    PeriodicTimeDistribution periodicDistribution(0, 400, 10);
    ParticleQuadGenerator generatorRain(10, glm::vec3(0.0f, -4.0f, 0.0f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 00.0f, 100.0f), periodicDistribution);

    glm::vec3 beginPos = glm::vec3(0.0f, -0.16f, -1.0f);
    glm::vec3 beginDir = glm::vec3(0.0f, 0.0f, -1.0f);
    ParticlePointGenerator pointGenerator(1, beginPos, beginDir, glm::radians(45.0f), 2.0f, constantDistribution);
    ParticleSystem particleSystem(generator, destroyer,  10000, std::string("resources/textures/snow.png"), 0.3f);

    ParticleSystem particleSystemPoint(pointGenerator, destroyerPoint,  2000, std::string("resources/textures/fire.png"), 0.3f);
    ParticleSystem rain(generatorRain, destroyerRain,  10000, std::string("resources/textures/raindrop.png"), 0.3f);
    particleSystem.initDraw();
    particleSystemPoint.initDraw();
    rain.initDraw();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        particleSystem.updateState(deltaTime);
        particleSystemPoint.updateState(deltaTime);
        rain.updateState(deltaTime);

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        glm::vec3 position = bspline.getCurrentPosition();
        glm::vec3 derivation = glm::normalize(bspline.getCurrentDerivation());
        glm::vec3 rotAxis = glm::cross(start, derivation);
        float angle = std::acos(glm::dot(derivation, start));


        bspline.draw(projection*view);
        particleSystem.draw(camera.Position, view, projection, (float)SCR_WIDTH / (float)SCR_HEIGHT);

        glm::vec3 tPos = glm::rotate(beginPos, angle, rotAxis) + position;
        pointGenerator.setPosition(tPos);
        glm::vec3 tDir = glm::rotate(beginDir, angle, rotAxis);
        pointGenerator.setDirection(tDir);
        particleSystemPoint.draw(camera.Position, view, projection, (float)SCR_WIDTH / (float)SCR_HEIGHT);
        rain.draw(camera.Position, view, projection, (float)SCR_WIDTH / (float)SCR_HEIGHT);
        
        
        glm::mat4 m(1.0f);
        m = glm::translate(m, position);
        m = glm::scale(m, glm::vec3(2.0f, 2.0f, 2.0f));
        m = glm::rotate(m, angle, rotAxis);
        model.draw(m, view, projection, camera.Position, lightPos);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        bspline.updateParameter(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        bspline.updateParameter(-deltaTime);

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        start = glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        start = glm::vec3(1.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        start = glm::vec3(1.0f, 0.0f, 1.0f);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        start = (-1.0f)*start;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


