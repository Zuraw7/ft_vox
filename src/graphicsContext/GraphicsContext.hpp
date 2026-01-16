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
    static int FPS();

    GraphicsContext() = delete;
private:
    static GLFWwindow *m_window;
    static double m_lastFrameTime;
    static double m_deltaTime;

    static int m_frameCount;
    static double m_fpsLastTime;
    static int m_currentFPS;

    static bool setupGLFW(int width, int height, const char *title);
    static void cleanupGLFW();
    static void setCallbacks();
    static bool setupGLAD();

    static void calculateDelta();
    static void calculateFPS();
};


#endif //FT_VOX_GRAPHICSCONTEXT_HPP
