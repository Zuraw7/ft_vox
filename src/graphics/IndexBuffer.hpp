#ifndef SCOP_INDEXBUFFER_HPP
#define SCOP_INDEXBUFFER_HPP
#include <vector>
#include <glad/gl.h>
#include <cstddef>

class IndexBuffer {
public:
    IndexBuffer() = delete;
    explicit IndexBuffer(const unsigned int *indices, const size_t size);
    explicit IndexBuffer(const std::vector<unsigned int> &indices);
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer &&other) noexcept;
    ~IndexBuffer();

    IndexBuffer &operator=(const IndexBuffer&) = delete;
    IndexBuffer &operator=(IndexBuffer &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_INDEXBUFFER_HPP