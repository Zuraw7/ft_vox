#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "camera/Camera.hpp"
#include "chunk/Chunk.hpp"
#include "wrappers/wrapGLFW.hpp"
#include "wrappers/wrapGLAD.hpp"
#include "shader/Shader.hpp"
#include "renderer/Renderer.hpp"
#include "textures/TextureManager.hpp"
#include "utils/declarations.hpp"
#include "object/Object.hpp"
#include "perlinNoise/PerlinNoise.hpp"

Camera gCamera(glm::vec3(0.0f, 80.0f, 3.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

std::unique_ptr<TextureManager> gTextureManager;
std::unique_ptr<PerlinNoise> gPerlinNoise;

void createChunks(std::vector<Chunk> &chunks)
{
    for (int x = 0; x < 20; x++) {
        for (int z = 0; z < 20; z++) {
            for (int y = 0; y < (256 / 16); y++) {
                glm::ivec3 chunkPos(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);

                chunks.emplace_back(chunkPos);
            }
        }
    }
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
        "../res/textures/blocks/stone.png",
        "../res/textures/blocks/bedrock.png",
        "../res/textures/blocks/brick.png",
        "../res/textures/blocks/diamond_ore.png",
        "../res/textures/blocks/bookshelf.png",
    };

    gTextureManager = std::make_unique<TextureManager>();
    for (const auto &path: texturePath) {
        gTextureManager->loadTexture2D(path);
    }

    gPerlinNoise = std::make_unique<PerlinNoise>(worldSeed);

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");

    std::vector<Chunk> chunks;
    createChunks(chunks);

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

        for (auto &chunk: chunks)
            chunk.draw(shader);

        glfwSwapBuffers(window);

        fprintf(stdout, "FPS: %f\n", 1.0 / deltaTime);
        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
