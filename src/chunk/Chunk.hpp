#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

class Shader;
class Mesh;
struct Vertex;

constexpr int CHUNK_SIZE = 16;

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    STONE,
    BEDROCK
};

class Chunk {
public:

    Chunk(const glm::ivec3 &chunkPos);

    void updateMesh();
    void uploadMesh();
    void draw(Shader &shader) const;

private:

    BlockType m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    glm::ivec3 m_pos;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::unique_ptr<Mesh> m_mesh;
};


#endif //FT_VOX_CHUNK_HPP