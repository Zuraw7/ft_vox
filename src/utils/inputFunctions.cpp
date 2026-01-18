#include <GLFW/glfw3.h>
#include "../camera/Camera.hpp"
#include "../inputManager/InputManager.hpp"

extern Camera gCamera;

void moveCamera(GLFWwindow *window, double deltaTime) {
    if (InputManager::isKeyHeld(GLFW_KEY_W))
        gCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
    if (InputManager::isKeyHeld(GLFW_KEY_S))
        gCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
    if (InputManager::isKeyHeld(GLFW_KEY_A))
        gCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
    if (InputManager::isKeyHeld(GLFW_KEY_D))
        gCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
    if (InputManager::isKeyHeld(GLFW_KEY_SPACE))
        gCamera.updateCameraPos(CameraDirection::UP, deltaTime);
    if (InputManager::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
        gCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
}

void rotateCamera(GLFWwindow *window) {
    const double x = InputManager::getMouseOffsetX();
    const double y = InputManager::getMouseOffsetY();
    if (x != 0 || y != 0)
        gCamera.updateCameraDirection(x, y);
    if (InputManager::getScrollOffsetY() != 0)
        gCamera.updateCameraZoom(InputManager::getScrollOffsetY());
}

void polygonModeManagement(GLFWwindow *window) {
    if (InputManager::isKeyPressed(GLFW_KEY_P))
        gCamera.switchPolygonMode();
}
