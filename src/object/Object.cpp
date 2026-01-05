#include "Object.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/detail/type_vec4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "../shader/Shader.hpp"
#include "../textures/TextureManager.hpp"

extern std::unique_ptr<TextureManager> gTextureManager;

Object::Object(const char *path) : m_model(glm::mat4(1.0f)), m_position(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {
    loadModel(path);
    updateModelMatrix();
}

Object::Object(Object &&other) noexcept : m_meshes(std::move(other.m_meshes)),
          m_texture(std::move(other.m_texture)),
          m_model(std::move(other.m_model)),
          m_position(std::move(other.m_position)),
          m_scale(std::move(other.m_scale)){

}

Object &Object::operator=(Object &&other) noexcept {
    if (this == &other)
        return *this;
    m_meshes = std::move(other.m_meshes);
    m_texture = std::move(other.m_texture);
    m_model = std::move(other.m_model);
    m_position = std::move(other.m_position);
    m_scale = std::move(other.m_scale);
    return *this;
}

void Object::draw(Shader &shader) const {
    for (const auto &mesh : m_meshes) {
        const unsigned int slot = gTextureManager->getSlot(m_texture);
        gTextureManager->bindTexture(m_texture);
        shader.setInt("uTexture", slot);

        mesh.draw(shader);
    }
}

void Object::setPosition(const glm::vec3 &pos) {
    m_position = pos;
    updateModelMatrix();
}

glm::mat4 &Object::getModel() {
    return m_model;
}

std::string & Object::getTexture() {
    return m_texture;
}

void Object::setTexture(const std::string &texturePath) {
    m_texture = texturePath;
}

void Object::setScale(const glm::vec3 scale) {
    m_scale = scale;
    updateModelMatrix();
}

void Object::loadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Object::processNode(const aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Object::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = texCoords;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices);
}

void Object::updateModelMatrix() {
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = glm::scale(m_model, m_scale);
}
