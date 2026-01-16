#include <random>
#include "camera/Camera.hpp"
#include "chunk/Chunk.hpp"
#include "chunk/ChunkManager.hpp"
#include "shader/Shader.hpp"
#include "renderer/Renderer.hpp"
#include "textures/TextureManager.hpp"
#include "utils/declarations.hpp"
#include "object/Object.hpp"
#include "graphicsContext/GraphicsContext.hpp"

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

    GraphicsContext::setup(currentRes.width, currentRes.height, "ft_vox");

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

    // Game LOOP
    while (!GraphicsContext::shouldClose()) {
        double deltaTime = GraphicsContext::deltaTime();

        processInput(GraphicsContext::window(), renderer, deltaTime);

        renderer.setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        shader.setUniformMatrix4fv("uView", gCamera.getCamView());
        shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());

        renderer.draw(chunkManager, shader, deltaTime);

        GraphicsContext::update();
        fprintf(stdout, "FPS: %d\n", GraphicsContext::FPS());
    }

    GraphicsContext::cleanup();

    return 0;
}
