#include <glad/gl.h>
#include "Texture2D.hpp"

std::shared_ptr<Texture2D> Texture2D::create(const std::string &path, const int wrapS, const int wrapT, const int minFilter, const int magFilter) {
    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
    texture->m_path = path;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texture->m_path.c_str(), &texture->m_width, &texture->m_height, &texture->m_nrChannels, 0);
    if (!data)
        return nullptr;

    glGenTextures(1, &texture->m_id);
    glBindTexture(GL_TEXTURE_2D, texture->m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    const int format = (texture->m_nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

Texture2D::Texture2D(Texture2D &&other) noexcept : m_id(other.m_id),
                                                   m_path(std::move(other.m_path)),
                                                   m_width(other.m_width),
                                                   m_height(other.m_height),
                                                   m_nrChannels(other.m_nrChannels) {
    other.m_id = 0;
    other.m_width = 0;
    other.m_height = 0;
    other.m_nrChannels = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
    if (this != &other) {
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nrChannels = other.m_nrChannels;
        m_path = std::move(other.m_path);
        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_nrChannels = 0;
    }
    return *this;
}

Texture2D::~Texture2D() {
    if (m_id != 0)
        glDeleteTextures(1, &m_id);
}

void Texture2D::bind(const unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

std::string Texture2D::getPath() const {
    return m_path;
}
