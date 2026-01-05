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

Camera gCamera(glm::vec3(0.0f, 1.0f, 3.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

std::unique_ptr<TextureManager> gTextureManager;

int main () {
    Resolution currentRes = FHD;

    // Prepare GLFW and GLAD
    GLFWwindow *window = wrapGLFW::init(currentRes.width, currentRes.height, "ft_vox");
    if (!window)
        return 1;

    if (!wrapGLAD::init()) {
        wrapGLFW::exit(window);
        return 1;
    }

    std::string texturePath[] = {
        "../res/textures/blocks/bedrock.png",
        "../res/textures/blocks/brick.png",
        "../res/textures/blocks/ice.png",
        "../res/textures/blocks/diamond_ore.png",
        "../res/textures/blocks/bookshelf.png",
        "../res/textures/blocks/dirt.png",
    };

    gTextureManager = std::make_unique<TextureManager>();
    for (const auto &path: texturePath) {
        gTextureManager->loadTexture2D(path);
    }

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");

    std::vector<std::unique_ptr<Object>> objects;

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[0]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(0.0f, 1.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[1]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(0.0f, 2.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[2]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(1.0f, 2.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[3]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[4]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

    objects.push_back(std::make_unique<Object>("../res/objects/Cube.obj"));
    objects.at(objects.size() - 1)->setPosition(glm::vec3(1.0f, 0.0f, 1.0f));
    objects.at(objects.size() - 1)->setTexture(texturePath[5]);
    objects.at(objects.size() - 1)->setScale(glm::vec3(0.5f));

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

        for (auto &object: objects) {
            renderer.draw(object, shader, deltaTime);
        }
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
