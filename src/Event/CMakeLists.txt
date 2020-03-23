cmake_minimum_required(VERSION 3.13)

target_sources(
    gl_base
    PUBLIC
        Event.h
        EventHandler.h
        InputEvents.h
        KeyDefinitions.h
        Listener.h
    PRIVATE
        EventHandler.cpp
        InputEvents.cpp
)

target_include_directories(
    gl_base
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
