#ifndef FT_VOX_INPUTFUNCTIONS_HPP
#define FT_VOX_INPUTFUNCTIONS_HPP

#include <GLFW/glfw3.h>

void moveCamera(GLFWwindow *window, double deltaTime);
void rotateCamera(GLFWwindow *window);
void polygonModeManagement(GLFWwindow *window);

#endif //FT_VOX_INPUTFUNCTIONS_HPP