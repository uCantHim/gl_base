cmake_minimum_required(VERSION 3.13)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(DevIL REQUIRED)

target_link_libraries(
    gl_base
    PUBLIC
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${IL_LIBRARIES}
        glfw
        pthread
)

target_include_directories(
    gl_base
    PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${IL_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/gsl_include/"
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    gl_base
    PUBLIC
        Camera.h
        GlmUtility.h
        OpenglResource.h
        Shader.h
        ShaderLoader.h
        Texture.h
        Window.h
    PRIVATE
        Camera.cpp
        Shader.cpp
        ShaderLoader.cpp
        Texture.cpp
        Window.cpp
)

# This has to be called after the sources have been specified
add_subdirectory(Event)
