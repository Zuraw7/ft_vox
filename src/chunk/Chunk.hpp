#ifndef FT_VOX_CHUNK_HPP
#define FT_VOX_CHUNK_HPP

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/ext/matrix_transform.hpp>

class Shader;
class Mesh;
struct Vertex;

constexpr int CHUNK_SIZE = 16;
constexpr int WORLD_HEIGHT = 256;
constexpr int WORLD_SIZE = 16384;

const std::vector<std::pair<float, float>> continentalPoints {
            {-1.0f, 50},
            {-0.5f, 60},
            {0.0f, 70},
            {0.5f, 90},
            {1.0f, 110}
};

const std::vector<std::pair<float, float>> erosionPoints {
        {-1.0f, 30},
        {-0.3f, 40},
        {0.3f, 45},
        {0.8f, 50},
        {1.0f, 55}
};

const std::vector<std::pair<float, float>> peaksAndValleysPoints {
        {-1.0f, 70},
        {-0.6f, 90},
        {0.2f, 110},
        {0.6f, 130},
        {1.0, 150}
};

enum class FaceDirection {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    STONE,
    BEDROCK
};

class Chunk {
public:

    Chunk(const glm::ivec2 &chunkPos);

    void updateMesh();
    void uploadMesh();
    void draw(Shader &shader) const;

private:

    BlockType m_blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE];
    glm::ivec2 m_pos;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::unique_ptr<Mesh> m_mesh;

    void addFace(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::ivec3 blockPos, FaceDirection dir);
};


#endif //FT_VOX_CHUNK_HPP