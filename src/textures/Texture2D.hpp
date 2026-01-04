#ifndef FT_VOX_TEXTURE_HPP
#define FT_VOX_TEXTURE_HPP

#include <iostream>
#include <memory>

class Texture2D {
public:
    Texture2D() = default;
    static std::shared_ptr<Texture2D> create(const std::string &path);
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


#endif //FT_VOX_TEXTURE_HPP