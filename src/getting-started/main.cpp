#include <vector>

#include "shader.hpp"
#include "opengl_object.hpp"
#include "window.hpp"  // Also includes glad and GLFW
#include "camera.hpp"

#include "ext/glm/glm.hpp"
#include "ext/glm/gtc/matrix_transform.hpp"
#include "ext/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

#define INFO_LOG_BUFFER_SIZE 512

struct TextureData
{
    uint textureId;
    GLenum textureUnit;
    const char* uniformName;
};

void initTexture(uint* textureId, const char* imagePath, GLenum imageColourFormat);
inline void processKeyboardInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow *, double, double yOffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;   // Time between the current frame and last frame
float lastFrame = 0.0f;

int main()
{
    const std::vector rectVertices = {
        // positions       // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
    };

    const std::vector cubeVertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    constexpr glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    const std::vector<uint> indices = {
        0, 1, 2, // First triangle
        2, 3, 0, // Second triangle
    };

    const std::vector triangleVertices = {
        // positions       // texture coords
        0.0f, -0.5f, 0.0f, 0.0f, 0.0f, // left
        0.2f, 0.5f, 0.0f, 0.5f, 1.0f, // top
        0.4f, -0.5f, 0.0f, 1.0f, 0.0f, // right
    };

    const std::vector<VBOConfig> vboConfig = {
        {0, 3}, // Position
        {1, 2}, // 2D Texture Coords
    };

    std::cout << "Running program...\n";
    GLFWwindow* window;
    initWindow(&window);

    const std::vector objects = {
        // OpenGLObject(triangleVertices, vboConfig)
        // OpenGLObject(rectVertices, indices, vboConfig),
        OpenGLObject(cubeVertices, vboConfig),
    };

    const auto shader = Shader("shaders/shader.vert", "shaders/shader.frag");

    uint containerTex, faceTex;
    initTexture(&containerTex, "textures/container.jpg", GL_RGB);
    initTexture(&faceTex, "textures/awesome_face.png", GL_RGBA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    const std::vector<TextureData> textures = {
        {containerTex, GL_TEXTURE0, "texture1"},
        {faceTex, GL_TEXTURE1, "texture2"},
    };

    // We currently only have one shader, so we can set it
    // as the active shader outside the render loop
    shader.use();

    for (int i = 0; i < textures.size(); i++)
    {
        shader.setInt(textures[i].uniformName, i);
    }

    for (const TextureData t : textures)
    {
        glActiveTexture(t.textureUnit);
        glBindTexture(GL_TEXTURE_2D, t.textureId);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeyboardInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();

        auto projection =
            glm::perspective(glm::radians(45.0f),
                             static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                             0.1f,
                             100.0f);

        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        for (int i = 0; i < 10; i++)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            const float angle = 20.0f * static_cast<float>(i + 1);
            model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(angle),
                                glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setMat4("model", glm::value_ptr(model));
            for (const OpenGLObject& object : objects)
            {
                object.draw();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Program terminated." << std::endl;
    return 0;
}

void initTexture(uint* textureId, const char* imagePath, const GLenum imageColourFormat)
{
    int width, height, channels;

    // OpenGL expects the (0,0) coordinate of an image to be on the bottom-left,
    // but usually in images it is on the top left. Flipping the image vertically
    // will make OpenGL render it in the intended orientation.
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(imagePath, &width, &height, &channels, 0);

    if (!data)
    {
        std::cout << "Failed to load texture at: " << imagePath << std::endl;
        stbi_image_free(data);
        exit(1);
    }

    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);

    // Set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageColourFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
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

inline void processKeyboardInput(GLFWwindow *window)
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

