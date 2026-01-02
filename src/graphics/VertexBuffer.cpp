#include "VertexBuffer.hpp"
#include "../core/Object.hpp"

VertexBuffer::VertexBuffer(const float *vertices, const size_t size) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<float> &vertices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<Vertex> &vertices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

VertexBuffer::~VertexBuffer() {
    if (m_id)
        glDeleteBuffers(1, &m_id);
}

VertexBuffer & VertexBuffer::operator=(VertexBuffer &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
