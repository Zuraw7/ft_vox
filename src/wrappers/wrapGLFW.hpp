#ifndef FT_VOX_WRAPGLFW_HPP
#define FT_VOX_WRAPGLFW_HPP

#include <GLFW/glfw3.h>

namespace wrapGLFW {
    GLFWwindow *init(int width, int height, const char *title);
    // setCallbacks
    void exit(GLFWwindow *window);
    void setCallbacks(GLFWwindow *window);
}

#endif //FT_VOX_WRAPGLFW_HPP