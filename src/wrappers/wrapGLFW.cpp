#include "wrapGLFW.hpp"
#include "../utils/callbacks.hpp"

namespace wrapGLFW {
    GLFWwindow *init(const int width, const int height, const char *title) {
        if (!glfwInit()) {
            printf("Failed to init GLFW\n");
            return nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            printf("Failed to create wrappers window\n");
            return nullptr;
        }

        glfwMakeContextCurrent(window);

        glfwSetWindowAspectRatio(window, 16, 9);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        glfwSwapInterval(0);

        return window;
    }

    void exit(GLFWwindow *window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}