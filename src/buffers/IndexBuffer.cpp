#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int *indices, const size_t size) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int> &indices) : m_id(0) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

IndexBuffer::~IndexBuffer() {
    if (m_id)
        glDeleteBuffers(1, &m_id);
}

IndexBuffer & IndexBuffer::operator=(IndexBuffer &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}