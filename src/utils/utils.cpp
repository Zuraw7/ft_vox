#include <glad/gl.h>
#include <iostream>

#include "../chunk/Chunk.hpp"

void checkGLError()
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout << err;
    }
}
