#include <GLFW/glfw3.h>
#include "../renderer/Renderer.hpp"
#include "../camera/Camera.hpp"

extern Camera gCamera;

static void moveCamera(GLFWwindow *window, double deltaTime);
static void polygonModeManagement(GLFWwindow *window);

void processInput(GLFWwindow *window, Renderer &renderer, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    moveCamera(window, deltaTime);
    polygonModeManagement(window);
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        gCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
    }
}


static void polygonModeManagement(GLFWwindow *window) {
    static bool prevP = false;
    const bool currP = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;

    if (currP && !prevP)
        gCamera.switchPolygonMode();
    prevP = currP;
}
