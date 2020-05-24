target_sources(
    gl_base
    PRIVATE
        Camera.cpp
        Shader.cpp
        ShaderLoader.cpp
        Texture.cpp
        Window.cpp
)

target_include_directories(
    gl_base
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

# This has to be called after the sources have been specified
add_subdirectory(event)
