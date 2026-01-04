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

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");
    Object object("../res/objects/Cube.obj");
    Renderer renderer;

    const char *texturePath = "../res/textures/blocks/bedrock.png";

    TextureManager textureManager;
    textureManager.loadTexture2D(texturePath);

    textureManager.bindTexture(texturePath);
    shader.setInt("uTexture", textureManager.getSlot(texturePath));

    double lastFrame = glfwGetTime();
    // Game LOOP
    while (!glfwWindowShouldClose(window)) {
        double thisFrame = glfwGetTime();
        double deltaTime = thisFrame - lastFrame;
        lastFrame = thisFrame;

        processInput(window, renderer, deltaTime);

        renderer.setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        renderer.setUniforms(shader);
        object.draw(shader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
