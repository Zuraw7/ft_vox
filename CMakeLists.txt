cmake_minimum_required(VERSION 3.20)
project(ft_vox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

add_executable(ft_vox
        src/main.cpp
        src/buffers/VertexArray.cpp
        src/buffers/VertexBuffer.cpp
        src/buffers/IndexBuffer.cpp
        src/camera/Camera.cpp
        src/chunk/Chunk.cpp
        src/chunk/ChunkManager.cpp
        src/graphicsContext/GraphicsContext.cpp
        src/inputManager/InputManager.cpp
        src/object/Mesh.cpp
        src/object/Object.cpp
        src/renderer/Renderer.cpp
        src/shader/Shader.cpp
        src/textures/Texture2D.cpp
        src/textures/TextureManager.cpp
        src/threadPool/ThreadPool.cpp
        src/utils/declarations.cpp
        src/utils/inputFunctions.cpp
        src/utils/utils.cpp
        include/glad/gl.c
        include/stb/stb_image.c
)

set_target_properties(ft_vox PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exec
)

target_include_directories(ft_vox
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)

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
        ZLIB::ZLIB
)