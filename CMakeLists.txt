cmake_minimum_required(VERSION 3.13)

# Project
project(opengl_base_utilities)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# External packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

# Library
add_library(gl_base)
add_subdirectory(src)
add_subdirectory(include)

target_link_libraries(
    gl_base
    PUBLIC
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        GLEW::GLEW
        glfw
        pthread
)

target_include_directories(
    gl_base
    PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
)

target_compile_options(
    gl_base
    PUBLIC
        -fPIC
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
)

target_compile_features(gl_base PUBLIC cxx_std_17)
