#ifndef FT_VOX_CHUNKMANAGER_HPP
#define FT_VOX_CHUNKMANAGER_HPP
#include <memory>
#include <mutex>
#include <vector>

#include "fnl/fnl.hpp"

enum class FaceDirection;
enum class BlockType : uint8_t;
class Shader;
class Chunk;

class ChunkManager {
public:
    ChunkManager(int playerX, int playerZ, int worldSeed);
    ~ChunkManager() = default;

    void drawChunks(Shader &shader) const;

private:
    std::vector<std::unique_ptr<Chunk>> m_chunks;
    std::mutex m_chunksMutex;

    void setupNoises(int worldSeed);
    void generateStartingChunks(int playerX, int playerZ);
    void calculateChunk(int x, int startingZ);
    void updateMeshes();
    void uploadChunksToGPU();
    // void renderChunks(); // Newly discovered by player
    // void removeChunks(); // Those that are not in render distance

};


#endif //FT_VOX_CHUNKMANAGER_HPP