#include "lighting.hpp"

#include "camera.hpp"
#include "opengl_object.hpp"
#include "shader.hpp"
#include "window.hpp"

#include "ext/glm/glm.hpp"
#include "ext/glm/gtc/matrix_transform.hpp"
#include "ext/glm/gtc/type_ptr.hpp"

void processKeyboardInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow *, double, double yOffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;   // Time between the current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    GLFWwindow* window;
    initWindow(&window);

    const auto cube = OpenGLObject(cubeVertices, cubeVboConfigList);
    const auto cubeShader = Shader("shaders/cube.vert", "shaders/cube.frag");
    const auto light = OpenGLObject(cubeVertices, cubeVboConfigList);
    const auto lightShader = Shader("shaders/light.vert", "shaders/light.frag");

    constexpr auto objectColour = glm::vec3(1.0f, 0.5f, 0.31f);
    constexpr auto lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processKeyboardInput(window);

        glm::mat4 view = camera.getViewMatrix();
        auto projection =
            glm::perspective(glm::radians(45.0f),
                             static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                             0.1f,
                             100.0f);
        auto cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.0f, 0.0f));

        cubeShader.use();
        cubeShader.setVec3("objectColour", glm::value_ptr(objectColour));
        cubeShader.setVec3("lightColour", glm::value_ptr(lightColour));
        cubeShader.setMat4("view", glm::value_ptr(view));
        cubeShader.setMat4("projection", glm::value_ptr(projection));
        cubeShader.setMat4("model", glm::value_ptr(cubeModel));

        cube.draw();

        auto lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        lightShader.use();
        lightShader.setMat4("view", glm::value_ptr(view));
        lightShader.setMat4("projection", glm::value_ptr(projection));
        lightShader.setMat4("model", glm::value_ptr(lightModel));

        light.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processKeyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Right, deltaTime);
}

void mouseCallback(GLFWwindow *, const double xPos, const double yPos)
{
    const auto x = static_cast<float>(xPos);
    const auto y = static_cast<float>(yPos);
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    const float xOffset = x - lastX;
    const float yOffset = lastY - y; // reversed since y-coordinates go from bottom to top
    lastX = x;
    lastY = y;

    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *, const double, const double yOffset)
{
    camera.processMouseScroll(static_cast<float>(yOffset));
}