#include "wrapGLAD.hpp"
#include <iostream>
namespace wrapGLAD {
    int init() {
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0) {
            printf("Failed to initialized OpenGL context\n");
            glfwTerminate();
            return 0;
        }
        printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_MULTISAMPLE);

        return version;
    }
}