#include <cstdio>
#include <glad/gl.h>
#include "GraphicsContext.hpp"
#include "../utils/callbacks.hpp"

GLFWwindow* GraphicsContext::m_window = nullptr;
double GraphicsContext::m_lastFrameTime = 0.0;
double GraphicsContext::m_deltaTime = 0.0;

double GraphicsContext::m_fpsLastTime = 0.0;
int GraphicsContext::m_frameCount = 0;
int GraphicsContext::m_currentFPS = 0;

bool GraphicsContext::setup(const int width, const int height, const char *title) {
    if (!setupGLFW(width, height, title))
        return false;
    if (!setupGLAD()) {
        cleanupGLFW();
        return false;
    }
    return true;
}

void GraphicsContext::cleanup() {
    cleanupGLFW();
}

bool GraphicsContext::shouldClose() {
    return glfwWindowShouldClose(m_window);
}

void GraphicsContext::update() {
    calculateDelta();
    calculateFPS();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

GLFWwindow *GraphicsContext::window() {
    return m_window;
}

double GraphicsContext::deltaTime() {
    return m_deltaTime;
}

int GraphicsContext::FPS() {
    return m_currentFPS;
}

bool GraphicsContext::setupGLFW(const int width, const int height, const char *title) {
    if (!glfwInit()) {
        printf("Failed to init GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        printf("Failed to create GLFW window\n");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    setCallbacks();
    glfwSetWindowAspectRatio(m_window, 16, 9);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(0);
    return true;
}

void GraphicsContext::cleanupGLFW() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GraphicsContext::setCallbacks() {
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
}

bool GraphicsContext::setupGLAD() {
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialized OpenGL context\n");
        return false;
    }
    printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

void GraphicsContext::calculateDelta() {
    double currentTime = glfwGetTime();
    m_deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
}

void GraphicsContext::calculateFPS() {
    m_frameCount++;
    double currentTime = glfwGetTime();
    if (currentTime - m_fpsLastTime >= 1.0) {
        m_currentFPS = m_frameCount;
        m_frameCount = 0;
        m_fpsLastTime = currentTime;
    }
}
