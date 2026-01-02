#include "TextureManager.hpp"

TextureManager::TextureManager() : m_nextSlot(0), m_maxSlots(0) {
    int maxUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
    m_maxSlots = static_cast<unsigned int>(maxUnits);
}

TextureManager::TextureManager(const TextureManager &other) : m_textures2D(other.m_textures2D), m_slots(other.m_slots), m_nextSlot(other.m_nextSlot) {
}

TextureManager::TextureManager(TextureManager &&other) noexcept : m_textures2D(std::move(other.m_textures2D)),
      m_slots(std::move(other.m_slots)),
      m_nextSlot(other.m_nextSlot) {
}

TextureManager &TextureManager::operator=(const TextureManager &other) {
    if (this == &other)
        return *this;
    m_textures2D = other.m_textures2D;
    m_slots = other.m_slots;
    m_nextSlot = other.m_nextSlot;
    return *this;
}

TextureManager & TextureManager::operator=(TextureManager &&other) noexcept {
    if (this == &other)
        return *this;
    m_textures2D = std::move(other.m_textures2D);
    m_slots = std::move(other.m_slots);
    m_nextSlot = other.m_nextSlot;
    return *this;
}

std::shared_ptr<Texture2D> TextureManager::loadTexture2D(const std::string &path, const int wrapS, const int wrapT, const int minFilter, const int magFilter) {
    const auto it = m_textures2D.find(path);
    if (it != m_textures2D.end())
        return it->second;

    auto tex = Texture2D::create(path, wrapS, wrapT, minFilter, magFilter);
    if (!tex) {
        fprintf(stderr, "Failed to load texture: %s\n", path.c_str());
        return nullptr;
    }
    m_textures2D[path] = tex;
    m_slots[path] = m_nextSlot;
    m_nextSlot++;
    return tex;
}

unsigned int TextureManager::getSlot(const std::string &path) {
    const auto it = m_slots.find(path);
    if (it != m_slots.end()) {
        return it->second;
    }
    fprintf(stderr, "Warning: texture %s not found, returning slot 0!\n", path.c_str());
    return 0;
}

void TextureManager::bindTexture(const std::string &path) {
    const auto it = m_textures2D.find(path);
    if (it != m_textures2D.end()) {
        const unsigned int slot = m_slots[path];
        it->second->bind(slot % m_maxSlots);
    }
}
