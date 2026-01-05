#ifndef FT_VOX_OBJECT_HPP
#define FT_VOX_OBJECT_HPP

#include <vector>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_float4x4.hpp>
#include "Mesh.hpp"
#include "../textures/Texture2D.hpp"

class Shader;
class Mesh;

class Object {
public:

    Object(const char *path);
    Object(Object &&other) noexcept;
    Object(const Object &other) = delete;

    Object &operator=(Object &&other) noexcept;
    Object &operator=(const Object &other) = delete;

    void draw(Shader &shader) const;

    glm::mat4 &getModel();
    std::string &getTexture();

    void setPosition(const glm::vec3 &pos);
    void setTexture(const std::string &texturePath);
    void setScale(glm::vec3 scale);

private:
    std::vector<Mesh> m_meshes;
    std::string m_texture;
    glm::mat4 m_model;
    glm::vec3 m_position;
    glm::vec3 m_scale;

    void loadModel(const std::string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void updateModelMatrix();
};


#endif //FT_VOX_OBJECT_HPP