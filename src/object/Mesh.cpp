#include "Mesh.hpp"
#include "../shader/Shader.hpp"
#include "../buffers/VertexArray.hpp"
#include "../buffers/VertexBuffer.hpp"
#include "../buffers/IndexBuffer.hpp"
#include <utility>
#include <glad/gl.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    m_vertices = std::move(vertices);
    m_indices = std::move(indices);
    m_textures = std::move(textures);

    setupMesh();
}

void Mesh::draw(Shader &shader) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    m_VAO->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_VAO->unbind();
}

void Mesh::setupMesh() {
    m_VAO = std::make_unique<VertexArray>(VertexArray());
    m_VBO = std::make_unique<VertexBuffer>(VertexBuffer(m_vertices));
    m_EBO = std::make_unique<IndexBuffer>(IndexBuffer(m_indices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    m_VAO->unbind();
}
