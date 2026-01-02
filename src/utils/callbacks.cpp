#include <glad/gl.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}
