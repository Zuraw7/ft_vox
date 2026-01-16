#ifndef FT_VOX_RENDERER_HPP
#define FT_VOX_RENDERER_HPP

#include <memory>
#include <vector>

class ChunkManager;
class Shader;
class Object;

class Renderer {
public:
    void setBackgroundColor(const float red, const float green, const float blue, const float alpha);
    void clear() const;
    void draw(ChunkManager &chunkManager, Shader& shader, double deltaTime) const;

private:
    void setUniforms(const std::unique_ptr<Object> &object, Shader& shader) const;
};

#endif //FT_VOX_RENDERER_HPP
