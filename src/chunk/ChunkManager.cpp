#include <algorithm>
#include "ChunkManager.hpp"
#include "Chunk.hpp"
#include "fnl/fnl.hpp"
#include "../object/Mesh.hpp"
#include "../utils/utils.hpp"

// Self first, then the four side neighbours whose border faces depend on this chunk's data.
static constexpr glm::ivec2 neighbourOffsets[] = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::unique_ptr<FastNoiseLite> gContinentalNoise;
std::unique_ptr<FastNoiseLite> gErosionNoise;
std::unique_ptr<FastNoiseLite> gPeaksNoise;

ChunkManager::ChunkManager(int playerX, int playerZ, int worldSeed) : m_playerPos( {playerX, playerZ} ) {
    setupNoises(worldSeed);
    generateStartingChunks();
    getStartingChunksToRender();
    updateStartingMeshes();
    uploadChunksToGPU();
}

void ChunkManager::drawChunks(Shader &shader) const {
    for (auto &chunk : m_chunksToRender)
        chunk->draw(shader);
}

void ChunkManager::update(const glm::ivec2 &playerPos) {
    glm::ivec2 oldChunk = worldToChunk(m_playerPos);
    glm::ivec2 newChunk = worldToChunk(playerPos);

    if (newChunk != oldChunk) {
        removeFarChunksMeshes();
        removeFarChunksFromCache();
        m_playerPos = playerPos;
        prepareChunksToGenerate(oldChunk);
        enqueueVisibleCachedChunks();
    }

    generateChunksAroundPlayer();
    flushCompletedChunks();
    flushVisibleChunks();
    uploadChunksToGPU();
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

void ChunkManager::calculateStartingChunk(const int x, const int startingZ) {
    const int chunkX = chunkToWorld(x);
    std::vector<std::pair<glm::ivec2, std::unique_ptr<Chunk>>> localChunks;
    for (int z = startingZ - CACHE_DISTANCE; z <= startingZ + CACHE_DISTANCE; z++) {
        glm::ivec2 chunkPos(chunkX, chunkToWorld(z));
        localChunks.emplace_back(glm::ivec2(x, z), std::make_unique<Chunk>(chunkPos));
    }
    std::lock_guard<std::mutex> lock(m_chunksMutex);
    for (auto &pair: localChunks)
        m_chunksInCache[pair.first] = std::move(pair.second);
}

void ChunkManager::generateStartingChunks() {
    int startingX = worldToChunk(m_playerPos.x);
    int startingZ = worldToChunk(m_playerPos.y);
    for (int x = startingX - CACHE_DISTANCE; x <= startingX + CACHE_DISTANCE; x++) {
        m_threadPool.enqueue([this, x, startingZ]() {
            this->calculateStartingChunk(x, startingZ);
        });
    }
    m_threadPool.waitAll();
}

void ChunkManager::updateStartingMeshes() {
    for (auto &chunk : m_chunksToRender) {
        m_threadPool.enqueue([this, chunk]() {
            int chunkX = worldToChunk(chunk->getPos().x);
            int chunkZ = worldToChunk(chunk->getPos().y);

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
    m_threadPool.waitAll();
}

void ChunkManager::getStartingChunksToRender() {
    m_chunksToRender.clear();
    for (auto &entry: m_chunksInCache) {
        if (entry.second) {
            entry.second->isVisible = false;
        }
    }

    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    for (int r = 0; r <= RENDER_DISTANCE; r++) {
        if (r == 0) {
            tryAddChunk(playerX, playerZ);
            continue;
        }

        int minX = playerX - r;
        int maxX = playerX + r;
        int minZ = playerZ - r;
        int maxZ = playerZ + r;

        // Up and down line
        for (int x = minX; x <= maxX; x++) {
            tryAddChunk(x, minZ);
            tryAddChunk(x, maxZ);
        }
        // Left and right line
        for (int z = minZ + 1; z < maxZ; z++) {
            tryAddChunk(minX, z);
            tryAddChunk(maxX, z);
        }
    }
}

void ChunkManager::uploadChunksToGPU() {
    for (auto &chunk: m_chunksToRender) {
        if (!chunk->isUploaded) {
            chunk->uploadMesh();
            chunk->isUploaded = true;
        }
    }
}

void ChunkManager::prepareChunksToGenerate(const glm::ivec2 &oldChunk) {
    glm::ivec2 delta = worldToChunk(m_playerPos) - oldChunk;
    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    if (delta.x != 0) {
        int newEdgeX = playerX + (delta.x > 0 ? CACHE_DISTANCE : -CACHE_DISTANCE);
        for (int z = playerZ - CACHE_DISTANCE; z <= playerZ + CACHE_DISTANCE; z++) {
            if (m_chunksInCache.find({newEdgeX, z}) == m_chunksInCache.end())
                m_chunksToGenerate.emplace_back(newEdgeX, z);
        }
    }
    if (delta.y != 0) {
        int newEdgeZ = playerZ + (delta.y > 0 ? CACHE_DISTANCE : -CACHE_DISTANCE);
        for (int x = playerX - CACHE_DISTANCE; x <= playerX + CACHE_DISTANCE; x++) {
            if (m_chunksInCache.find({x, newEdgeZ}) == m_chunksInCache.end())
                m_chunksToGenerate.emplace_back(x, newEdgeZ);
        }
    }
}

void ChunkManager::generateChunksAroundPlayer() {
    int generatedThisFrame = 0;

    while (!m_chunksToGenerate.empty() && generatedThisFrame < MAX_PER_FRAME) {
        glm::ivec2 coord = m_chunksToGenerate.back();
        m_chunksToGenerate.pop_back();
        m_threadPool.enqueue([this, coord] {
            auto chunk = std::make_unique<Chunk>(chunkToWorld(coord));
            std::unique_lock<std::mutex> lock(m_chunksMutex);
            m_generatedChunks.push({coord, std::move(chunk)});
        });
        generatedThisFrame++;
    }
}

void ChunkManager::tryAddChunk(int x, int z) {
    auto it = m_chunksInCache.find({x, z});
    if (it != m_chunksInCache.end() && !it->second->isVisible) {
        it->second->isVisible = true;
        m_chunksToRender.emplace_back(it->second.get());
    }
}

void ChunkManager::removeFarChunksMeshes() {
    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    for (auto& chunk: m_chunksInCache) {
        int dx = std::abs(chunk.first.x - playerX);
        int dz = std::abs(chunk.first.y - playerZ);

        if (dx > RENDER_DISTANCE || dz > RENDER_DISTANCE) {
            if (chunk.second->isVisible) {
                chunk.second->isVisible = false;
                chunk.second->isUploaded = false;
                chunk.second->clearMesh();
            }
        }
    }

    m_chunksToRender.erase(
        std::remove_if(m_chunksToRender.begin(), m_chunksToRender.end(),
            [](Chunk *c) { return !c->isVisible; }),
            m_chunksToRender.end()
            );
}

void ChunkManager::removeFarChunksFromCache() {
    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    for (auto it = m_chunksInCache.begin(); it != m_chunksInCache.end(); ) {
        int dx = std::abs(it->first.x - playerX);
        int dz = std::abs(it->first.y - playerZ);

        if (dx > CACHE_DISTANCE || dz > CACHE_DISTANCE) {
            it->second->isVisible = false;
            it->second->isUploaded = false;

            m_chunksToRender.erase(
                std::remove(m_chunksToRender.begin(), m_chunksToRender.end(), it->second.get()),
                m_chunksToRender.end());
            it = m_chunksInCache.erase(it);
        } else {
            ++it;
        }
    }
}

void ChunkManager::enqueueVisibleCachedChunks() {
    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    for (int x = playerX - RENDER_DISTANCE; x <= playerX + RENDER_DISTANCE; x++) {
        for (int z = playerZ - RENDER_DISTANCE; z <= playerZ + RENDER_DISTANCE; z++) {
            auto it = m_chunksInCache.find({x, z});
            if (it != m_chunksInCache.end() && !it->second->isVisible)
                m_dirtyMeshes.insert({x, z});
        }
    }
}

void ChunkManager::flushVisibleChunks() {
    if (m_dirtyMeshes.empty())
        return;

    int playerX = worldToChunk(m_playerPos.x);
    int playerZ = worldToChunk(m_playerPos.y);

    for (auto it = m_dirtyMeshes.begin(); it != m_dirtyMeshes.end();) {
        int dx = std::abs(it->x - playerX);
        int dz = std::abs(it->y - playerZ);

        if (dx > RENDER_DISTANCE || dz > RENDER_DISTANCE) {
            it = m_dirtyMeshes.erase(it);
            continue;
        }

        auto cacheIt = m_chunksInCache.find(*it);
        if (cacheIt == m_chunksInCache.end()) {
            it = m_dirtyMeshes.erase(it);
            continue;
        }

        Chunk *chunk = cacheIt->second.get();
        auto getChunk = [this](int x, int z) -> Chunk* {
            auto nb = m_chunksInCache.find({x, z});
            return nb != m_chunksInCache.end() ? nb->second.get() : nullptr;
        };

        Chunk* left  = getChunk(it->x - 1, it->y);
        Chunk* right = getChunk(it->x + 1, it->y);
        Chunk* front = getChunk(it->x, it->y + 1);
        Chunk* back  = getChunk(it->x, it->y - 1);

        if (!left || !right || !front || !back) {
            ++it;
            continue;
        }

        chunk->updateMesh(left, right, front, back);

        if (!chunk->isVisible) {
            chunk->isVisible = true;
            m_chunksToRender.emplace_back(chunk);
        }

        it = m_dirtyMeshes.erase(it);
    }
}

void ChunkManager::flushCompletedChunks() {
    std::queue<std::pair<glm::ivec2, std::unique_ptr<Chunk>>> completed;
    {
        std::lock_guard<std::mutex> lock(m_chunksMutex);
        completed.swap(m_generatedChunks);
    }

    while (!completed.empty()) {
        auto chunkInfo = std::move(completed.front());
        completed.pop();
        if (m_chunksInCache.find(chunkInfo.first) == m_chunksInCache.end()) {
            m_chunksInCache[chunkInfo.first] = std::move(chunkInfo.second);
            for (const auto &offset: neighbourOffsets)
                m_dirtyMeshes.insert(chunkInfo.first + offset);
        }
    }
}
