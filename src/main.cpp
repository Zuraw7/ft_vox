#include <random>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fnl/fnl.hpp>

#include "camera/Camera.hpp"
#include "chunk/Chunk.hpp"
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
std::unique_ptr<FastNoiseLite> gContinentalNoise;
std::unique_ptr<FastNoiseLite> gErosionNoise;
std::unique_ptr<FastNoiseLite> gPeaksNoise;

void setupFnl(int worldSeed) {
    gContinentalNoise = std::make_unique<FastNoiseLite>();
    gContinentalNoise->SetSeed(worldSeed);
    gContinentalNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gContinentalNoise->SetFractalType(FastNoiseLite::FractalType_FBm);
    gErosionNoise = std::make_unique<FastNoiseLite>();
    gErosionNoise->SetSeed(worldSeed + 200);
    gErosionNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gErosionNoise->SetFractalType(FastNoiseLite::FractalType_FBm);
    gPeaksNoise = std::make_unique<FastNoiseLite>();
    gPeaksNoise->SetSeed(worldSeed + 300);
    gPeaksNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gPeaksNoise->SetFractalType(FastNoiseLite::FractalType_FBm);
}

void createChunks(float x, float z, std::vector<Chunk> &chunks)
{
    int xPos = x / 16;
    int zPos = z / 16;

    for (int x = xPos - 8; x < xPos + 8; x++) {
        for (int z = zPos - 8; z < zPos + 8; z++) {
            glm::ivec2 chunkPos(x * CHUNK_SIZE, z * CHUNK_SIZE);
            chunks.emplace_back(chunkPos);
        }
    }
}

// TODO: After creating player character -> move to class
glm::vec2 getSpawn(const int worldSeed) {
    std::mt19937 rng(worldSeed);
    std::uniform_int_distribution<int> distX(-(WORLD_SIZE / 2), WORLD_SIZE / 2);
    std::uniform_int_distribution<int> distZ(-(WORLD_SIZE / 2), WORLD_SIZE / 2);
    int x = distX(rng);
    int z = distZ(rng);

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

    setupFnl(worldSeed);
    std::vector<Chunk> chunks;

    glm::vec2 spawnPoint = getSpawn(worldSeed);
    gCamera.setCameraPosition({spawnPoint.x, gCamera.getPosition().y ,spawnPoint.y});
    createChunks(gCamera.getPosition().x, gCamera.getPosition().z, chunks);

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

        renderer.draw(chunks, shader, deltaTime);

        glfwSwapBuffers(window);

        // fprintf(stdout, "FPS: %f\n", 1.0 / deltaTime);
        glfwPollEvents();
    }

    // Cleanup
    wrapGLFW::exit(window);

    return 0;
}
