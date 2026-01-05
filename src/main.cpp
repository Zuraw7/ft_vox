#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "camera/Camera.hpp"
#include "wrappers/wrapGLFW.hpp"
#include "wrappers/wrapGLAD.hpp"
#include "shader/Shader.hpp"
#include "renderer/Renderer.hpp"
#include "textures/TextureManager.hpp"
#include "utils/declarations.hpp"
#include "object/Object.hpp"

Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

std::unique_ptr<TextureManager> gTextureManager;

int main () {
    Resolution currentRes = HD;

    // Prepare GLFW and GLAD
    GLFWwindow *window = wrapGLFW::init(currentRes.width, currentRes.height, "ft_vox");
    if (!window)
        return 1;

    if (!wrapGLAD::init()) {
        wrapGLFW::exit(window);
        return 1;
    }

    gTextureManager = std::make_unique<TextureManager>();
    std::string texturePath = "../res/textures/blocks/bedrock.png";
    gTextureManager->loadTexture2D(texturePath);

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");

    std::unique_ptr<Object> object = std::make_unique<Object>("../res/objects/Cube.obj");
    object->setWorldPosition(glm::vec3(1.0f, 1.0f, 1.0f));
    object->setTexture(texturePath);

    Renderer renderer;

    double lastFrame = glfwGetTime();
    // Game LOOP
    while (!glfwWindowShouldClose(window)) {
        double thisFrame = glfwGetTime();
        double deltaTime = thisFrame - lastFrame;
        lastFrame = thisFrame;

        processInput(window, renderer, deltaTime);

        renderer.setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        renderer.draw(object, shader, deltaTime);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
