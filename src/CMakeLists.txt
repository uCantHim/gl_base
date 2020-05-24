cmake_minimum_required(VERSION 3.13)

target_include_directories(
    gl_base
    PUBLIC
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
