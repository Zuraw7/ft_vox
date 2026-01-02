#include "Renderer.hpp"
#include "../camera/Camera.hpp"
#include "../textures/TextureManager.hpp"

extern Camera gCamera;

void Renderer::setBackgroundColor(const float red, const float green, const float blue, const float alpha) {
    glClearColor(red, green, blue, alpha);
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// void Renderer::draw(std::unique_ptr<Object> &object, Shader &shader, float deltaTime) const {
//     static float rColorMix = 1.0f;
//
//     object->bind();
//     shader.bind();
//
//     setUniforms(object, shader);
//
//     glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode ? GL_LINE : GL_FILL);
//
//     glDrawElements(GL_TRIANGLES, object->getIndices().size(), GL_UNSIGNED_INT, nullptr);
//
//     object->unbind();
//     shader.unbind();
// }

void Renderer::setUniforms(/*std::unique_ptr<Object> &object,*/ Shader &shader) const {
    // shader.setUniformMatrix4fv("uModel", object->getMatrix());

    glm::mat4 model = glm::mat4(1.0f);
    shader.setUniformMatrix4fv("uModel", model);
    shader.setUniformMatrix4fv("uView", gCamera.getCamView());
    shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());

    // std::string texturePath = object->getTexture2DPath();
    // gTextureManager->bindTexture(texturePath);
    // shader.setInt("aTexture", gTextureManager->getSlot(texturePath));

    // object->getMaterial()->apply(shader);
}
