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
    void draw(Shader &shader) const;

    glm::mat4 &getModel();
    std::string &getTexture();

    void setWorldPosition(const glm::vec3 &worldPos);
    void setTexture(const std::string &texturePath);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_loadedTextures;
    std::string m_directory;
    glm::mat4 m_model;
    std::string m_texture;

    void loadModel(const std::string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};


#endif //FT_VOX_OBJECT_HPP