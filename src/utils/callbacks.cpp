#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../camera/Camera.hpp"

extern Camera gCamera;

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

bool firstMouse = true;
double lastX = 0.0f;
double lastY = 0.0f;

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double offsetX = xpos - lastX;
    double offsetY = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    if (offsetX != 0 || offsetY != 0)
        gCamera.updateCameraDirection(offsetX, offsetY);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    gCamera.updateCameraZoom(yoffset);
}
