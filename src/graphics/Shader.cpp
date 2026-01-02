#include <fstream>
#include <sstream>
#include "Shader.hpp"

std::string parseShaderFile(const std::string &path) {
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        fprintf(stderr, "Error opening file!\n");
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void checkShaderCompilationStatus(unsigned int shader, unsigned int type) {
    const char *shaderName;
    switch (type) {
        case GL_VERTEX_SHADER:
            shaderName = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderName = "fragment";
            break;
        default:
            shaderName = "unknown";
    }

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Error compiling %s shader: %s\n", shaderName, infoLog);
    }
}

Shader::Shader(const std::string &vertex, const std::string &fragment) : m_id(0), m_vertexShader(0), m_fragmentShader(0) {
    m_vertexShader = compileShader(vertex, GL_VERTEX_SHADER);
    m_fragmentShader = compileShader(fragment, GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, m_vertexShader);
    glAttachShader(m_id, m_fragmentShader);
    glLinkProgram(m_id);

    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
}

Shader::Shader(Shader &&other) noexcept : m_id(other.m_id), m_vertexShader(other.m_vertexShader), m_fragmentShader(other.m_fragmentShader) {
    other.m_id = 0;
    other.m_vertexShader = 0;
    other.m_fragmentShader = 0;
}

Shader::~Shader() {
    if (m_id)
        glDeleteProgram(m_id);
}

Shader & Shader::operator=(Shader &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    m_vertexShader = other.m_vertexShader;
    m_fragmentShader = other.m_fragmentShader;
    other.m_id = 0;
    other.m_vertexShader = 0;
    other.m_fragmentShader = 0;
    return *this;
}

void Shader::bind() const {
    glUseProgram(m_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

unsigned int Shader::getId() const {
    return m_id;
}

void Shader::setUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix) {
    bind();

    const int location = findLoc(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec3(const std::string &name, const glm::vec3 &vec) {
    bind();

    const int location = findLoc(name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::setInt(const std::string &name, int value) {
    bind();

    const int location = findLoc(name);
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) {
    bind();

    const int location = findLoc(name);
    glUniform1f(location, value);
}

unsigned int Shader::compileShader(const std::string &path, unsigned int type) {
    std::string source = parseShaderFile(path);
    if (source.empty()) {
        fprintf(stderr, "Shader file '%s' is empty!\n", path.c_str());
        return 0;
    }

    const char *src = source.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    checkShaderCompilationStatus(shader, type);

    return shader;
}

int Shader::findLoc(const std::string &name) {
    int location;
    const auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        location = m_uniformCache[name];
    } else {
        location = glGetUniformLocation(m_id, name.c_str());
        m_uniformCache[name] = location;
    }
    return location;
}
