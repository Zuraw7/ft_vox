#ifndef FT_VOX_RENDERER_HPP
#define FT_VOX_RENDERER_HPP

// #include "../core/Object.hpp"
#include "../graphics/Shader.hpp"

// class Object;

/**
 * @brief Renderer wraps all rendering calls into dedicated functions. It also controlls how objects are being rendered.
 */
class Renderer {
public:
    void setBackgroundColor(const float red, const float green, const float blue, const float alpha);
    void clear() const;
    // void draw(std::unique_ptr<Object> &object, Shader& shader, float deltaTime) const;

    void setUniforms(/*std::unique_ptr<Object> &object,*/ Shader& shader) const;
private:
};

#endif //FT_VOX_RENDERER_HPP
