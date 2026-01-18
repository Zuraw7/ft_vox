#ifndef FT_VOX_INPUTMANAGER_HPP
#define FT_VOX_INPUTMANAGER_HPP

#include <GLFW/glfw3.h>
#include <array>

class InputManager {
public:

    InputManager() = delete;

    // Management
    static void init(GLFWwindow *window);
    static void processInput(GLFWwindow *window, double deltaTime);

    // Callbacks
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Key state
    static bool isKeyPressed(int key);
    static bool isKeyHeld(int key);
    static bool isKeyReleased(int key);

    // Mouse getters
    static double getMouseOffsetX();
    static double getMouseOffsetY();
    static double getScrollOffsetY();

private:
    static void update();
    static void resetMouseOffsets();

    // Keyboard state
    static std::array<bool, GLFW_KEY_LAST + 1> m_currentKey;
    static std::array<bool, GLFW_KEY_LAST + 1> m_previousKey;

    // Mouse state
    static double m_mouseLastX, m_mouseLastY;
    static bool m_firstMouse;
    static double m_mouseOffsetX, m_mouseOffsetY;
    static double m_scrollOffsetY;
};


#endif //FT_VOX_INPUTMANAGER_HPP