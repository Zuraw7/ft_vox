#ifndef FT_VOX_GRAPHICSCONTEXT_HPP
#define FT_VOX_GRAPHICSCONTEXT_HPP
#include <GLFW/glfw3.h>

class GraphicsContext {
public:
    static bool setup(int width, int height, const char *title);
    static void cleanup();
    static bool shouldClose();
    static void update();
    static GLFWwindow *window();

    static double deltaTime();
    [[deprecated("Use avgFPS() for more stable, smoothed value.")]] static int FPS();
    static double avgFPS();

    GraphicsContext() = delete;
private:
    static GLFWwindow *m_window;
    static double m_lastFrameTime;
    static double m_deltaTime;

    static int m_frameCount;
    static double m_fpsLastTime;
    static int m_currentFPS;
    static double m_avgFPS;

    static bool setupGLFW(int width, int height, const char *title);
    static void cleanupGLFW();
    static bool setupGLAD();

    static void framebufferSizeCallback(GLFWwindow* window, const int width, const int height);

    static void calculateDelta();
    static void calculateFPS();
    static void calculateAvgFPS();
};


#endif //FT_VOX_GRAPHICSCONTEXT_HPP
