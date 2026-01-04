#ifndef FT_VOX_VERTEXARRAY_HPP
#define FT_VOX_VERTEXARRAY_HPP

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


#endif //FT_VOX_VERTEXARRAY_HPP