#include <thread>
#include <map>
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "fnl/fnl.hpp"
#include "../object/Mesh.hpp"

std::unique_ptr<FastNoiseLite> gContinentalNoise;
std::unique_ptr<FastNoiseLite> gErosionNoise;
std::unique_ptr<FastNoiseLite> gPeaksNoise;

ChunkManager::ChunkManager(int playerX, int playerZ, int worldSeed) : m_playerPos( {playerX, playerZ} ) {
    setupNoises(worldSeed);
    generateStartingChunks();
    getChunksToRender();
    updateMeshes();
    uploadChunksToGPU();
}

void ChunkManager::drawChunks(Shader &shader) const {
    for (auto &chunk : m_chunksToRender)
        chunk->draw(shader);
}

void ChunkManager::update(const glm::ivec2 &playerPos) {
    /* Jeżeli gracz zmienił pozycję to:
        1. Zaktualizuj pozycję gracza
        2. Wygeneruj nowe chunki w cache'u
        3. Oblicz face'y dla nowo widocznych Chunków
        4. Przestań wyświetlać chunki poza render distance -> usuń z GPU
        5. Usuń chunki spoza cache'a
    */
    if (playerPos.x != m_playerPos.x || playerPos.y != m_playerPos.y) {
        m_playerPos = playerPos;
        // generateChunks();
        // updateMeshes();
        // uploadChunksToGPU(); // zmienić tak żeby tylko niewidoczne chunki były dodawane
        // moveChunksToCache();
        // removeChunks();
    }
}

void ChunkManager::setupNoises(int worldSeed) {
    gContinentalNoise = std::make_unique<FastNoiseLite>();
    gContinentalNoise->SetSeed(worldSeed);
    gContinentalNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gContinentalNoise->SetFractalType(FastNoiseLite::FractalType_FBm);

    gContinentalNoise->SetFrequency(0.001f);
    gContinentalNoise->SetFractalOctaves(3);
    gContinentalNoise->SetFractalGain(0.5f);
    gContinentalNoise->SetFractalLacunarity(2.0f);

    gErosionNoise = std::make_unique<FastNoiseLite>();
    gErosionNoise->SetSeed(worldSeed);
    gErosionNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gErosionNoise->SetFractalType(FastNoiseLite::FractalType_FBm);

    gErosionNoise->SetFrequency(0.004f);
    gErosionNoise->SetFractalOctaves(4);
    gErosionNoise->SetFractalGain(0.45f);
    gErosionNoise->SetFractalLacunarity(2.0f);

    gPeaksNoise = std::make_unique<FastNoiseLite>();
    gPeaksNoise->SetSeed(worldSeed);
    gPeaksNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    gPeaksNoise->SetFractalType(FastNoiseLite::FractalType_FBm);

    gPeaksNoise->SetFrequency(0.01f);
    gPeaksNoise->SetFractalOctaves(5);
    gPeaksNoise->SetFractalGain(0.5f);
    gPeaksNoise->SetFractalLacunarity(2.2f);
}

void ChunkManager::calculateChunk(const int x, const int startingZ) {
    const int chunkX = x * CHUNK_SIZE;
    std::vector<std::pair<glm::ivec2, std::unique_ptr<Chunk>>> localChunks;
    for (int z = startingZ - CACHE_DISTANCE; z < startingZ + CACHE_DISTANCE; z++) {
        glm::ivec2 chunkPos(chunkX, z * CHUNK_SIZE);
        localChunks.emplace_back(glm::ivec2(x, z), std::make_unique<Chunk>(chunkPos));
    }
    std::lock_guard<std::mutex> lock(m_chunksMutex);
    for (auto &pair: localChunks)
        m_chunksInCache[pair.first] = std::move(pair.second);
}

void ChunkManager::generateStartingChunks() {
    int startingX = m_playerPos.x / CHUNK_SIZE;
    int startingZ = m_playerPos.y / CHUNK_SIZE;
    std::vector<std::thread> threads;
    for (int x = startingX - CACHE_DISTANCE; x < startingX + CACHE_DISTANCE; x++) {
        threads.emplace_back([this, x, startingZ]() {
            this->calculateChunk(x, startingZ);
        });
    }
    for (auto &t: threads)
        t.join();
}

void ChunkManager::getChunksToRender() {
    m_chunksToRender.clear();
    int playerX = m_playerPos.x / CHUNK_SIZE;
    int playerZ = m_playerPos.y / CHUNK_SIZE;
    for (auto &chunk : m_chunksInCache) {
        int chunkX = chunk.first.x;
        int chunkZ = chunk.first.y;
        if ((playerX + RENDER_DISTANCE >= chunkX && playerX - RENDER_DISTANCE <= chunkX) && (playerZ + RENDER_DISTANCE >= chunkZ && playerZ - RENDER_DISTANCE <= chunkZ))
            m_chunksToRender.emplace_back(chunk.second.get());
    }
}

void ChunkManager::updateMeshes() {

    std::vector<std::thread> threads;
    for (auto &chunk : m_chunksToRender) {
        threads.emplace_back([this, &chunk]() {
            int chunkX = chunk->getPos().x / CHUNK_SIZE;
            int chunkZ = chunk->getPos().y / CHUNK_SIZE;

            auto it = m_chunksInCache.find(glm::ivec2(chunkX - 1, chunkZ));
            Chunk* left = (it != m_chunksInCache.end()) ? it->second.get() : nullptr;

            it = m_chunksInCache.find(glm::ivec2(chunkX + 1, chunkZ));
            Chunk* right = (it != m_chunksInCache.end()) ? it->second.get() : nullptr;

            it = m_chunksInCache.find(glm::ivec2(chunkX, chunkZ + 1));
            Chunk* front = (it != m_chunksInCache.end()) ? it->second.get() : nullptr;

            it = m_chunksInCache.find(glm::ivec2(chunkX, chunkZ - 1));
            Chunk* back = (it != m_chunksInCache.end()) ? it->second.get() : nullptr;

            chunk->updateMesh(left, right, front, back);
        });
    }

    for (auto &t : threads)
        t.join();
}

void ChunkManager::uploadChunksToGPU() {
    for (auto &chunk: m_chunksToRender) {
        chunk->uploadMesh();
    }
}
