#ifndef SCOP_VERTEXBUFFER_HPP
#define SCOP_VERTEXBUFFER_HPP
#include <vector>
#include <glad/gl.h>
#include <cstddef>

struct Vertex;

class VertexBuffer {
public:
    VertexBuffer() = delete;
    explicit VertexBuffer(const float *vertices, const size_t size);
    explicit VertexBuffer(const std::vector<float> &vertices);
    explicit VertexBuffer(const std::vector<Vertex> &vertices);
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer &&other) noexcept;
    ~VertexBuffer();

    VertexBuffer &operator=(const VertexBuffer&) = delete;
    VertexBuffer &operator=(VertexBuffer &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_VERTEXBUFFER_HPP