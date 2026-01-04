#include <GLFW/glfw3.h>
#include "../renderer/Renderer.hpp"
#include "../camera/Camera.hpp"

extern Camera gCamera;

static void moveCamera(GLFWwindow *window, double deltaTime);

void processInput(GLFWwindow *window, Renderer &renderer, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    moveCamera(window, deltaTime);
}

static void moveCamera(GLFWwindow *window, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
    }
}