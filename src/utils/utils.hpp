#ifndef FT_VOX_UTILS_HPP
#define FT_VOX_UTILS_HPP

// #include "../renderer/Renderer.hpp"
#include <GLFW/glfw3.h>

class Renderer;

void checkGLError();
void processInput(GLFWwindow *window, Renderer &renderer, double deltaTime);

#endif //FT_VOX_UTILS_HPP