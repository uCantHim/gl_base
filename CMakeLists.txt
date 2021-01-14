cmake_minimum_required(VERSION 3.13)

# Project
project(opengl_base_utilities)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library
add_library(gl_base)
add_subdirectory(src)
add_subdirectory(include)

# External packages
if(NOT WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm REQUIRED)
    find_package(glfw3 REQUIRED)

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
else()
    find_package(OpenGL REQUIRED)
    find_package(glm REQUIRED HINTS ${CMAKE_CURRENT_LIST_DIR}/external/glm/cmake/)

    target_include_directories(
        gl_base PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/external/glew-2.2.0/include/
        ${CMAKE_CURRENT_LIST_DIR}/external/glfw-3.3.2.bin.WIN64/include/
    )
    target_link_libraries(
        gl_base PUBLIC
        ${OPENGL_LIBRARIES}
        ${CMAKE_CURRENT_LIST_DIR}/external/glew-2.2.0/lib/Release/x64/glew32s.lib
        ${CMAKE_CURRENT_LIST_DIR}/external/glfw-3.3.2.bin.WIN64/lib-vc2019/glfw3.lib
    )
    target_compile_definitions(gl_base PUBLIC GLEW_STATIC)
endif()

target_compile_options(
    gl_base
    PUBLIC
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-fPIC>
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
)

target_compile_features(gl_base PUBLIC cxx_std_17)
