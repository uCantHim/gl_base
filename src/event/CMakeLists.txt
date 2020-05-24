target_sources(
    gl_base
    PRIVATE
        EventHandler.cpp
        InputEvents.cpp
)

target_include_directories(gl_base PUBLIC ${CMAKE_CURRENT_LIST_DIR})
