#include <glm/ext/matrix_transform.hpp>
#include "Renderer.hpp"

#include <memory>

#include "../camera/Camera.hpp"
#include "../chunk/Chunk.hpp"
#include "../shader/Shader.hpp"
#include "../object/Object.hpp"
#include "../textures/TextureManager.hpp"

extern Camera gCamera;
extern std::unique_ptr<TextureManager> gTextureManager;

void Renderer::setBackgroundColor(const float red, const float green, const float blue, const float alpha) {
    glClearColor(red, green, blue, alpha);
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(std::vector<Chunk> &chunks, Shader &shader, float deltaTime) const {

    shader.bind();

    unsigned int mode = GL_FILL;
    if (gCamera.getPolygonMode())
        mode = GL_LINE;
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    for (const auto &chunk: chunks)
        chunk.draw(shader);

    shader.unbind();
}

void Renderer::setUniforms(const std::unique_ptr<Object> &object, Shader &shader) const {
    shader.setUniformMatrix4fv("uModel", object->getModel());
    shader.setUniformMatrix4fv("uView", gCamera.getCamView());
    shader.setUniformMatrix4fv("uProjection", gCamera.getCamProjection());
}
