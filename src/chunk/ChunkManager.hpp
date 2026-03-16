#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "glm/vec2.hpp"

enum class FaceDirection;
enum class BlockType : uint8_t;
class Shader;
class Chunk;

struct ivec2Hash {
    std::size_t operator()(const glm::ivec2& v) const noexcept {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

class ChunkManager {
public:
    ChunkManager(int playerX, int playerZ, int worldSeed);
    ~ChunkManager() = default;

    void drawChunks(Shader &shader) const;
    void update(const glm::ivec2 &playerPos);

private:
    glm::ivec2 m_playerPos;
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2Hash> m_chunksInCache;
    std::vector<Chunk *> m_chunksToRender;
    std::vector<glm::ivec2> m_chunksToMakeVisible;
    std::vector<glm::ivec2> m_chunksToGenerate;
    std::mutex m_chunksMutex;

    // Methods to call on create
    void setupNoises(int worldSeed);
    void calculateStartingChunk(int x, int startingZ);
    void generateStartingChunks();
    void tryAddChunk(int x, int z);
    void getStartingChunksToRender();
    void updateStartingMeshes();

    // Render related
    void uploadChunksToGPU();

    // On chunk move
    void removeFarChunksMeshes();
    void removeFarChunksFromCache();
    void prepareChunksToGenerate();
    void generateChunksAroundPlayer();

    // Per frame
    void enqueueVisibleCachedChunks();
    void flushVisibleChunks();
};

#endif //FT_VOX_CHUNKMANAGER_HPP