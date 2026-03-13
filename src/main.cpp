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
#include "inputManager/InputManager.hpp"

Camera gCamera(glm::vec3(0.0f, 140.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

std::unique_ptr<TextureManager> gTextureManager;

// TODO: After creating player character -> move to class
glm::ivec2 getSpawn(const int worldSeed) {
    std::mt19937 rng(worldSeed);
    std::uniform_int_distribution<int> locate(-(WORLD_SIZE / 2), WORLD_SIZE / 2);
    int x = locate(rng);
    int z = locate(rng);

    return {x, z};
}

int main () {
    Resolution currentRes = FHD;
    int worldSeed = 2115;

    GraphicsContext::setup(currentRes.width, currentRes.height, "ft_vox");
    InputManager::init(GraphicsContext::window());

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

    glm::ivec2 spawnPoint = getSpawn(worldSeed);
    gCamera.setCameraPosition({spawnPoint.x, gCamera.getPosition().y ,spawnPoint.y});

    ChunkManager chunkManager(spawnPoint.x, spawnPoint.y, worldSeed);

    Renderer renderer;
    const unsigned int slot = gTextureManager->getSlot(texturePath[0]);
    gTextureManager->bindTexture(texturePath[0]);
    shader.setInt("uTexture", slot);

    // Game LOOP
    while (!GraphicsContext::shouldClose()) {
        double deltaTime = GraphicsContext::deltaTime();

        renderer.setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer.clear();

        shader.setUniformMatrix4fv("uView", gCamera.getCamView());
        shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());

        renderer.draw(chunkManager, shader, deltaTime);

        // chunkManager.update( {gCamera.getPosition().x, gCamera.getPosition().z} );
        GraphicsContext::update();
        fprintf(stdout, "avgFPS: %f\n", GraphicsContext::avgFPS());
    }

    GraphicsContext::cleanup();

    return 0;
}
