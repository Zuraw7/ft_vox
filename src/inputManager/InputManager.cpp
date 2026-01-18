#include "InputManager.hpp"
#include "../utils/inputFunctions.hpp"

std::array<bool, GLFW_KEY_LAST + 1> InputManager::m_currentKey;
std::array<bool, GLFW_KEY_LAST + 1> InputManager::m_previousKey;

double InputManager::m_mouseLastX = 0.0;
double InputManager::m_mouseLastY = 0.0;
bool InputManager::m_firstMouse = true;
double InputManager::m_mouseOffsetX;
double InputManager::m_mouseOffsetY;
double InputManager::m_scrollOffsetY;

void InputManager::init(GLFWwindow *window) {
    // Keyboard
    glfwSetKeyCallback(window, keyCallback);

    // Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void InputManager::processInput(GLFWwindow *window, double deltaTime) {
    if (isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    moveCamera(window, deltaTime);
    rotateCamera(window);
    polygonModeManagement(window);
    update();
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key > GLFW_KEY_LAST)
        return;

    if (action == GLFW_PRESS)
        m_currentKey[key] = true;

    if (action == GLFW_RELEASE)
        m_currentKey[key] = false;
}

void InputManager::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    if (m_firstMouse) {
        m_mouseLastX = xpos;
        m_mouseLastY = ypos;
        m_firstMouse = false;
    }

    m_mouseOffsetX = xpos - m_mouseLastX;
    m_mouseOffsetY = ypos - m_mouseLastY;

    m_mouseLastX = xpos;
    m_mouseLastY = ypos;
}

void InputManager::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    m_scrollOffsetY = yoffset;
}

bool InputManager::isKeyPressed(int key) {
    return m_currentKey[key] && !m_previousKey[key];
}

bool InputManager::isKeyHeld(int key) {
    return m_currentKey[key];
}

bool InputManager::isKeyReleased(int key) {
    return !m_currentKey[key] && m_previousKey[key];
}

double InputManager::getMouseOffsetX() {
    return m_mouseOffsetX;
}

double InputManager::getMouseOffsetY() {
    return m_mouseOffsetY;
}

double InputManager::getScrollOffsetY() {
    return m_scrollOffsetY;
}

void InputManager::update() {
    m_previousKey = m_currentKey;
    resetMouseOffsets();
}

void InputManager::resetMouseOffsets() {
    m_mouseOffsetX = 0.0;
    m_mouseOffsetY = 0.0;
    m_scrollOffsetY = 0.0;
}
