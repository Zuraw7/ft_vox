#ifndef SCOP_SHADER_HPP
#define SCOP_SHADER_HPP
#include <iostream>
#include <array>
#include <unordered_map>
#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader(const std::string &vertex, const std::string &fragment);
    Shader(const Shader& other) = delete;
    Shader(Shader &&other) noexcept;
    ~Shader();

    Shader &operator=(const Shader &other) = delete;
    Shader &operator=(Shader &&other) noexcept;

    void bind() const;
    void unbind() const;
    unsigned int getId() const;

    void setUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix);
    void setUniformVec3(const std::string &name, const glm::vec3 &vec);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

private:
    unsigned int m_id;
    std::unordered_map<std::string, int> m_uniformCache;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;

    unsigned int compileShader(const std::string &path, unsigned int type);
    int findLoc(const std::string &name);
};


#endif //SCOP_SHADER_HPP