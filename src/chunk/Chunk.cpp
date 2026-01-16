#include "Chunk.hpp"
#include "../object/Mesh.hpp"
#include "../shader/Shader.hpp"
#include <fnl/fnl.hpp>

extern std::unique_ptr<FastNoiseLite> gContinentalNoise;
extern std::unique_ptr<FastNoiseLite> gErosionNoise;
extern std::unique_ptr<FastNoiseLite> gPeaksNoise;

static float getPoint(float noise, const std::vector<std::pair<float, float>> &heights) {
    for (int i = 0; i < heights.size() - 1; i++) {
         if (heights[i].first <= noise && heights[i + 1].first >= noise) {
             float t = (noise - heights[i].first) / (heights[i + 1].first - heights[i].first);
             t = glm::smoothstep(0.0f, 1.0f, t);
             return glm::mix(heights[i].second, heights[i + 1].second, t);
         }
    }

    return heights.back().second;
}

BlockType getBlock(const int x, const int y, const int z) {
    float continentalness = gContinentalNoise->GetNoise(static_cast<float>(x), static_cast<float>(z));
    float erosion = gErosionNoise->GetNoise(static_cast<float>(x), static_cast<float>(z));
    float peaksAndValleys = gPeaksNoise->GetNoise(static_cast<float>(x), static_cast<float>(z));

    float continentalHeight = getPoint(continentalness, continentalPoints);
    float erosionHeight = getPoint(erosion, erosionPoints);
    float peaksAndValleysHeight = getPoint(peaksAndValleys, peaksAndValleysPoints);

    float mountainMask = glm::smoothstep(0.2f, 0.7f, continentalness);

    float surfaceY =  continentalHeight + erosionHeight * 0.4f + peaksAndValleysHeight * mountainMask;

    return y < surfaceY ? BlockType::STONE : BlockType::AIR;
}

// m_pos is glm::ivec2 (it contains x and y) so x = x and z = y
Chunk::Chunk(const glm::ivec2 &chunkPos) : m_pos(chunkPos) {

    for (int x = 0; x < CHUNK_SIZE; x++) {
        const int worldX = m_pos.x + x;

        for (int z = 0; z < CHUNK_SIZE; z++) {
            const int worldZ = m_pos.y + z;

            for (int y = 0; y < WORLD_HEIGHT; y++) {

                m_blocks[x][y][z] = getBlock(worldX, y, worldZ);
            }
        }
    }
}

Chunk::Chunk(Chunk &&other) noexcept
    : m_pos(std::move(other.m_pos)),
      m_vertices(std::move(other.m_vertices)),
      m_indices(std::move(other.m_indices)),
      m_mesh(std::move(other.m_mesh)) {
}

Chunk &Chunk::operator=(Chunk &&other) noexcept {
    if (this == &other)
        return *this;
    m_pos = std::move(other.m_pos);
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_mesh = std::move(other.m_mesh);
    return *this;
}

void Chunk::updateMesh(Chunk* left, Chunk* right, Chunk* front, Chunk* back) {
    m_vertices.clear();
    m_indices.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =  0; y < WORLD_HEIGHT; y++) {
                if (m_blocks[x][y][z] == BlockType::AIR)
                    continue;
                // Top
                if (y < WORLD_HEIGHT - 1 && m_blocks[x][y + 1][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::TOP);
                }

                // Bottom
                if (y == 0 || m_blocks[x][y - 1][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::BOTTOM);
                }

                // Left
                if (x == 0) {
                    if (!left || left->getBlockType(CHUNK_SIZE - 1, y, z) == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::LEFT);
                } else {
                    if (m_blocks[x - 1][y][z] == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::LEFT);
                }

                // Right
                if (x == CHUNK_SIZE - 1) {
                    if (!right || right->getBlockType(0, y, z) == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::RIGHT);
                } else {
                    if (m_blocks[x + 1][y][z] == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::RIGHT);
                }

                // Front
                if (z == CHUNK_SIZE - 1) {
                    if (!front || front->getBlockType(x, y, 0) == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::FRONT);
                } else {
                    if (m_blocks[x][y][z + 1] == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::FRONT);
                }

                // Back
                if (z == 0) {
                    if (!back || back->getBlockType(x, y, CHUNK_SIZE - 1) == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::BACK);
                } else {
                    if (m_blocks[x][y][z - 1] == BlockType::AIR)
                        addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::BACK);
                }
            }
        }
    }
}

void Chunk::uploadMesh() {
    m_mesh = std::make_unique<Mesh>(m_vertices, m_indices);
}

void Chunk::draw(Shader &shader) const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_pos.x, 0, m_pos.y));
    shader.setUniformMatrix4fv("uModel", model);
    if (m_mesh)
        m_mesh->draw(shader);
}

glm::ivec2 Chunk::getPos() const {
    return m_pos;
}

BlockType Chunk::getBlockType(int x, int y, int z) {
    return m_blocks[x][y][z];
}

void Chunk::addFace(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::ivec3 blockPos, FaceDirection dir) {
    uint32_t offset = vertices.size();
    glm::vec3 normal;
    glm::vec3 vert[4];

    switch (dir) {
        case FaceDirection::TOP:
            normal = {0, 1, 0};
            vert[0] = {0, 1, 0};
            vert[1] = {1, 1, 0};
            vert[2] = {1, 1, 1};
            vert[3] = {0, 1, 1};
            break;
        case FaceDirection::BOTTOM:
            normal = {0, -1, 0};
            vert[0] = {0, 0, 0};
            vert[1] = {0, 0, 1};
            vert[2] = {1, 0, 1};
            vert[3] = {1, 0, 0};
            break;
        case FaceDirection::LEFT:
            normal = {-1, 0, 0};
            vert[0] = {0, 0, 0};
            vert[1] = {0, 1, 0};
            vert[2] = {0, 1, 1};
            vert[3] = {0, 0, 1};
            break;
        case FaceDirection::RIGHT:
            normal = {1, 0, 0};
            vert[0] = {1, 0, 0};
            vert[1] = {1, 0, 1};
            vert[2] = {1, 1, 1};
            vert[3] = {1, 1, 0};
            break;
        case FaceDirection::FRONT:
            normal = {0, 0, 1};
            vert[0] = {0, 0, 1};
            vert[1] = {0, 1, 1};
            vert[2] = {1, 1, 1};
            vert[3] = {1, 0, 1};
            break;
        case FaceDirection::BACK:
            normal = {0, 0, -1};
            vert[0] = {0, 0, 0};
            vert[1] = {1, 0, 0};
            vert[2] = {1, 1, 0};
            vert[3] = {0, 1, 0};
            break;
    }

    glm::vec2 uv[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    for (int i = 0; i < 4; i++)
        vertices.push_back({glm::vec3(vert[i]) + glm::vec3(blockPos), normal, uv[i]});

    indices.push_back(offset + 0);
    indices.push_back(offset + 3);
    indices.push_back(offset + 2);
    indices.push_back(offset + 2);
    indices.push_back(offset + 1);
    indices.push_back(offset + 0);
}
