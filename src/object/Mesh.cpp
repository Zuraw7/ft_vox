#include "Mesh.hpp"
#include "../shader/Shader.hpp"
#include "../buffers/VertexArray.hpp"
#include "../buffers/VertexBuffer.hpp"
#include "../buffers/IndexBuffer.hpp"
#include <utility>
#include <glad/gl.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    m_vertices = std::move(vertices);
    m_indices = std::move(indices);

    setupMesh();
}

Mesh::Mesh(Mesh &&other) noexcept
    : m_vertices(std::move(other.m_vertices)),
      m_indices(std::move(other.m_indices)),
      m_VAO(std::move(other.m_VAO)),
      m_VBO(std::move(other.m_VBO)),
      m_EBO(std::move(other.m_EBO)) {
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
    if (this == &other)
        return *this;
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_VAO = std::move(other.m_VAO);
    m_VBO = std::move(other.m_VBO);
    m_EBO = std::move(other.m_EBO);
    return *this;
}

void Mesh::draw(Shader &shader) const {
    m_VAO->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    m_VAO->unbind();
}

void Mesh::setupMesh() {
    m_VAO = std::make_unique<VertexArray>(VertexArray());
    m_VBO = std::make_unique<VertexBuffer>(VertexBuffer(m_vertices));
    m_EBO = std::make_unique<IndexBuffer>(IndexBuffer(m_indices));

    m_VAO->bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, light)));

    m_VAO->unbind();
}
