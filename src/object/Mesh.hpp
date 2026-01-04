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
class VertexArray;
class VertexBuffer;
class IndexBuffer;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader &shader) const;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_EBO;

    void setupMesh();
};


#endif //FT_VOX_MESH_HPP