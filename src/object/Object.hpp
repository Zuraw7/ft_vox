#ifndef FT_VOX_OBJECT_HPP
#define FT_VOX_OBJECT_HPP

#include <vector>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "Mesh.hpp"

class Shader;
class Mesh;

class Object {
public:
    Object(const char *path);
    void draw(Shader &shader) const;

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_loadedTextures;

    void loadModel(const std::string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};


#endif //FT_VOX_OBJECT_HPP