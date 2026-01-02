#ifndef SCOP_TEXTURE_HPP
#define SCOP_TEXTURE_HPP

#include "../../include/stb/stb_image.h"
#include <iostream>
#include <memory>
#include <glad/gl.h>

class Texture2D {
public:
    Texture2D() = default;
    static std::shared_ptr<Texture2D> create(const std::string &path,
              int wrapS = GL_MIRRORED_REPEAT,
              int wrapT = GL_MIRRORED_REPEAT,
              int minFilter = GL_LINEAR_MIPMAP_LINEAR,
              int magFilter = GL_LINEAR);
    Texture2D(const Texture2D &other) = delete;
    Texture2D(Texture2D &&other) noexcept;

    Texture2D &operator=(const Texture2D &other) = delete;
    Texture2D &operator=(Texture2D &&other) noexcept;

    ~Texture2D();

    void bind(const unsigned int slot = 0) const;

    std::string getPath() const;

private:
    unsigned int m_id;
    std::string m_path;
    int m_width, m_height, m_nrChannels;
};


#endif //SCOP_TEXTURE_HPP