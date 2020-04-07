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

if (WIN32)
    target_link_libraries(
        gl_base
        opengl32
        glu32
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/glew_lib/GLEW_1130.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/glfw_lib/glfw3.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/il_lib/x64/Release/DevIL.lib"
    )

    target_include_directories(
        gl_base
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/glew_include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/glfw_include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/glm_include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/il_include"
    )
endif()

if (LINUX)
    target_link_libraries(
        gl_base
        GL
        GLEW
        glfw
        IL
        pthread
    )

    target_include_directories(
        gl_base
        PUBLIC
        "/usr/include/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../include/gsl_include/"
    )
endif()
