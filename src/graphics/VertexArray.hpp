#ifndef SCOP_VERTEXARRAY_HPP
#define SCOP_VERTEXARRAY_HPP
#include <glad/gl.h>

class VertexArray {
public:
    VertexArray();
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray &&other) noexcept;
    ~VertexArray();

    VertexArray &operator=(const VertexArray&) = delete;
    VertexArray &operator=(VertexArray &&other) noexcept;

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};


#endif //SCOP_VERTEXARRAY_HPP