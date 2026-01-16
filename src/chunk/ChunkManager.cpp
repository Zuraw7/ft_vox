#include <thread>
#include <map>
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "../object/Mesh.hpp"

std::unique_ptr<FastNoiseLite> gContinentalNoise;
std::unique_ptr<FastNoiseLite> gErosionNoise;
std::unique_ptr<FastNoiseLite> gPeaksNoise;

ChunkManager::ChunkManager(int playerX, int playerZ, int worldSeed) {
    setupNoises(worldSeed);
    generateStartingChunks(playerX, playerZ);
    updateMeshes();
    uploadChunksToGPU();
}

void ChunkManager::drawChunks(Shader &shader) const {
    for (auto &chunk : m_chunks)
        chunk->draw(shader);
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
    std::vector<std::unique_ptr<Chunk>> localChunks;
    for (int z = startingZ - RENDER_DISTANCE; z < startingZ + RENDER_DISTANCE; z++) {
        glm::ivec2 chunkPos(chunkX, z * CHUNK_SIZE);
        localChunks.emplace_back(std::make_unique<Chunk>(chunkPos));
    }
    std::lock_guard<std::mutex> lock(m_chunksMutex);
    for (auto &chunk: localChunks)
        m_chunks.push_back(std::move(chunk));
}

void ChunkManager::generateStartingChunks(const int playerX, const int playerZ) {
    int startingX = playerX / CHUNK_SIZE;
    int startingZ = playerZ / CHUNK_SIZE;
    std::vector<std::thread> threads;
    for (int x = startingX - RENDER_DISTANCE; x < startingX + RENDER_DISTANCE; x++) {
        threads.emplace_back([this, x, startingZ]() {
            this->calculateChunk(x, startingZ);
        });
    }
    for (auto &t: threads)
        t.join();
}

static std::map<std::pair<int, int>, Chunk*> getChunkMap(std::vector<std::unique_ptr<Chunk>> &chunks) {
    std::map<std::pair<int, int>, Chunk*> chunkMap;
    for (auto& c : chunks) {
        chunkMap[{c->getPos().x / CHUNK_SIZE, c->getPos().y / CHUNK_SIZE}] = c.get();
    }
    return chunkMap;
}

void ChunkManager::updateMeshes() {
    std::map<std::pair<int,int>, Chunk*> chunkMap = getChunkMap(m_chunks);

    std::vector<std::thread> threads;
    for (auto &chunk : m_chunks) {
        threads.emplace_back([&chunkMap, &chunk]() {
            int chunkX = chunk->getPos().x / CHUNK_SIZE;
            int chunkZ = chunk->getPos().y / CHUNK_SIZE;

            auto it = chunkMap.find({chunkX - 1, chunkZ});
            Chunk* left = (it != chunkMap.end()) ? it->second : nullptr;

            it = chunkMap.find({chunkX + 1, chunkZ});
            Chunk* right = (it != chunkMap.end()) ? it->second : nullptr;

            it = chunkMap.find({chunkX, chunkZ + 1});
            Chunk* front = (it != chunkMap.end()) ? it->second : nullptr;

            it = chunkMap.find({chunkX, chunkZ - 1});
            Chunk* back = (it != chunkMap.end()) ? it->second : nullptr;

            chunk->updateMesh(left, right, front, back);
        });
    }

    for (auto &t : threads)
        t.join();
}

void ChunkManager::uploadChunksToGPU() {
    for (auto &chunk: m_chunks) {
        chunk->uploadMesh();
    }
}
