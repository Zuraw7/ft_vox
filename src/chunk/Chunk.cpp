#include "Chunk.hpp"
#include "../object/Mesh.hpp"
#include "../shader/Shader.hpp"
#include "glm/ext/matrix_transform.hpp"

enum class FaceDirection {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

void addFace(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::ivec3 blockPos, FaceDirection dir);

Chunk::Chunk(const glm::ivec3 &chunkPos) : m_pos(chunkPos) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y =  0; y <  CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType block;
                if (y < 1)
                    block = BlockType::BEDROCK;
                else if (y < 5)
                    block = BlockType::STONE;
                else if (y < 10)
                    block = BlockType::DIRT;
                else
                    block = BlockType::AIR;
                m_blocks[x][y][z] = block;
            }
        }
    }
    updateMesh();
    uploadMesh();
}

void Chunk::updateMesh() {
    m_vertices.clear();
    m_indices.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y =  0; y <  CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (m_blocks[x][y][z] == BlockType::AIR)
                    continue;
                // Check for every neighbor if AIR
                // Top
                if (y == CHUNK_SIZE - 1 || m_blocks[x][y + 1][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::TOP);
                }

                // Bottom
                if (y == 0 || m_blocks[x][y - 1][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::BOTTOM);
                }

                // Left
                if (x == 0 || m_blocks[x - 1][y][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::LEFT);
                }

                // Right
                if (x == CHUNK_SIZE - 1 || m_blocks[x + 1][y][z] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::RIGHT);
                }

                // Front
                if (z == CHUNK_SIZE - 1 || m_blocks[x][y][z + 1] == BlockType::AIR) {
                    addFace(m_vertices, m_indices, glm::ivec3(x, y, z), FaceDirection::FRONT);
                }

                // Back
                if (z == 0 || m_blocks[x][y][z - 1] == BlockType::AIR) {
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
    model = glm::translate(model, glm::vec3(m_pos));
    shader.setUniformMatrix4fv("uModel", model);
    if (m_mesh)
        m_mesh->draw(shader);
}

void addFace(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::ivec3 blockPos, FaceDirection dir) {
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
