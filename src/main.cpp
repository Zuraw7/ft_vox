#include <random>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fnl/fnl.hpp>

#include "camera/Camera.hpp"
#include "chunk/Chunk.hpp"
#include "chunk/ChunkManager.hpp"
#include "wrappers/wrapGLFW.hpp"
#include "wrappers/wrapGLAD.hpp"
#include "shader/Shader.hpp"
#include "renderer/Renderer.hpp"
#include "textures/TextureManager.hpp"
#include "utils/declarations.hpp"
#include "object/Object.hpp"

Camera gCamera(glm::vec3(0.0f, 140.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

std::unique_ptr<TextureManager> gTextureManager;

// TODO: After creating player character -> move to class
glm::vec2 getSpawn(const int worldSeed) {
    std::mt19937 rng(worldSeed);
    std::uniform_int_distribution<int> locate(-(WORLD_SIZE / 2), WORLD_SIZE / 2);
    int x = locate(rng);
    int z = locate(rng);

    return glm::vec2(x, z);
}

int main () {
    Resolution currentRes = FHD;
    int worldSeed = 2115;

    // Prepare GLFW and GLAD
    GLFWwindow *window = wrapGLFW::init(currentRes.width, currentRes.height, "ft_vox");
    if (!window)
        return 1;

    if (!wrapGLAD::init()) {
        wrapGLFW::exit(window);
        return 1;
    }

    std::string texturePath[] = {
        "../res/textures/blocks/dirt.png",
        "../res/textures/blocks/beacon.png",
        "../res/textures/blocks/stone.png",
        "../res/textures/blocks/bedrock.png",
        "../res/textures/blocks/diamond_ore.png",
        "../res/textures/blocks/bookshelf.png",
    };

    gTextureManager = std::make_unique<TextureManager>();
    for (const auto &path: texturePath) {
        gTextureManager->loadTexture2D(path);
    }

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");

    glm::vec2 spawnPoint = getSpawn(worldSeed);
    gCamera.setCameraPosition({spawnPoint.x, gCamera.getPosition().y ,spawnPoint.y});

    auto now = std::chrono::system_clock::now();
    ChunkManager chunkManager(spawnPoint.x, spawnPoint.y, worldSeed);
    auto after = std::chrono::system_clock::now();
    auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(after - now);
    fprintf(stdout, "%d ms\n", dif);

    Renderer renderer;
    const unsigned int slot = gTextureManager->getSlot(texturePath[1]);
    gTextureManager->bindTexture(texturePath[1]);
    shader.setInt("uTexture", slot);

    double lastFrame = glfwGetTime();
    // Game LOOP
    while (!glfwWindowShouldClose(window)) {
        double thisFrame = glfwGetTime();
        double deltaTime = thisFrame - lastFrame;
        lastFrame = thisFrame;

        processInput(window, renderer, deltaTime);

        renderer.setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        shader.setUniformMatrix4fv("uView", gCamera.getCamView());
        shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());

        renderer.draw(chunkManager, shader, deltaTime);

        glfwSwapBuffers(window);

        // fprintf(stdout, "FPS: %f\n", 1.0 / deltaTime);
        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
