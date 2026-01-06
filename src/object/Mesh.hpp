#ifndef FT_VOX_MESH_HPP
#define FT_VOX_MESH_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "../buffers/VertexArray.hpp"
#include "../buffers/VertexBuffer.hpp"
#include "../buffers/IndexBuffer.hpp"

class Shader;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    void draw(Shader &shader) const;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_EBO;

    void setupMesh();
};


#endif //FT_VOX_MESH_HPP