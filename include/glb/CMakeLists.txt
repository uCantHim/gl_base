target_sources(
    gl_base
    PUBLIC
        Camera.h
        GlmUtility.h
        LazyInitializer.h
        OpenglResource.h
        Shader.h
        ShaderLoader.h
        Timer.h
        Window.h
)

target_include_directories(gl_base PRIVATE ${CMAKE_CURRENT_LIST_DIR})
