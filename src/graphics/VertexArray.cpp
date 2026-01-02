#include "VertexArray.hpp"

VertexArray::VertexArray() : m_id(0) {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

VertexArray::~VertexArray() {
    if (m_id)
        glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const {
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
