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
#include <Quad.hpp>


#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 960;

// camera

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
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

glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
glm::vec3 start(0.0f, 0.0f, 1.0f);

Quad* pQuad1, *pQuad2, *pQuad;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LAB3", NULL, NULL);
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

    Quad quad(glm::vec3(-1.0f, -1.0f, 0.0f),
              glm::vec3(2.0f, 0.0f, 0.0f),
              glm::vec3(0.0f, 2.0f, 0.0f),
              std::string("resources/textures/bricks2.jpg"),
              std::string("resources/textures/bricks2_normal.jpg"),
              std::string("resources/textures/bricks2_disp.jpg"));
    pQuad1 = &quad;


    Quad quad2(glm::vec3(-1.0f, -1.0f, 0.0f),
              glm::vec3(2.0f, 0.0f, 0.0f),
              glm::vec3(0.0f, 2.0f, 0.0f),
              std::string("resources/textures/wood.png"),
              std::string("resources/textures/toy_box_normal.png"),
              std::string("resources/textures/toy_box_disp.png"));
    pQuad2 = &quad2;

    // render loop
    // -----------

    pQuad = pQuad1;
    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

        glm::vec3 position(0.0f, 0.0f, 0.0f);



        glm::mat4 m(1.0f);
        m = glm::translate(m, position);
        m = glm::scale(m, glm::vec3(2.0f, 2.0f, 2.0f));
        pQuad->draw(m, view, projection, camera.Position, lightPos);

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

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        start = glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        start = glm::vec3(1.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        start = glm::vec3(1.0f, 0.0f, 1.0f);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        start = (-1.0f)*start;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        pQuad1->setType(0);
        pQuad2->setType(0);
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
        pQuad1->setType(1);
        pQuad2->setType(1);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        pQuad1->setType(2);
        pQuad2->setType(2);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        pQuad1->setType(3);
        pQuad2->setType(3);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        pQuad1->setType(4);
        pQuad2->setType(4);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        pQuad1->incrementScale(-0.01);
        pQuad2->incrementScale(-0.01);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        pQuad1->incrementScale(0.01);
        pQuad2->incrementScale(0.01);


    }


    static int ColdState = GLFW_RELEASE;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && ColdState == GLFW_RELEASE )
    {
        if(pQuad == pQuad1) pQuad = pQuad2;
        else pQuad = pQuad1;
    }
    ColdState = glfwGetKey(window, GLFW_KEY_C);

    static int JoldState = GLFW_RELEASE;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && JoldState == GLFW_RELEASE )
    {
        pQuad1->incrementNumLayers(-1);
        pQuad2->incrementNumLayers(-1);
    }
    JoldState = glfwGetKey(window, GLFW_KEY_J);

    static int KoldState = GLFW_RELEASE;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && KoldState == GLFW_RELEASE )
    {
        pQuad1->incrementNumLayers(1);
        pQuad2->incrementNumLayers(1);
    }
    KoldState = glfwGetKey(window, GLFW_KEY_K);
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


