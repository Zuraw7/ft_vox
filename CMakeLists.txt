cmake_minimum_required(VERSION 3.20)
project(ft_vox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

add_executable(ft_vox
        src/main.cpp
        src/gl.c
        src/camera/Camera.cpp
        src/graphics/Shader.cpp
        src/render/Renderer.cpp
        src/textures/Texture2D.cpp
        src/textures/TextureManager.cpp
        src/utils/callbacks.cpp
        src/utils/declarations.cpp
        src/utils/utils.cpp
        src/wrappers/wrapGLFW.cpp
        src/wrappers/wrapGLAD.cpp
        include/stb/stb_image.c
)

target_include_directories(ft_vox
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)

add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)

add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libassimp.a
)

target_link_libraries(ft_vox
    PRIVATE
        glfw3
        OpenGL::GL
        assimp
)