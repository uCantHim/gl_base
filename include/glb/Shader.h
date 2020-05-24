#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <stdexcept>

#include <GL/glew.h>

#include "OpenglResource.h"

namespace glb
{
    class shader_error : public std::exception
    {
    public:
        explicit shader_error(std::string errorString)
            : error(std::move(errorString)) {}

        [[nodiscard]]
        auto what() const noexcept -> const char* override {
            return error.c_str();
        }

    private:
        std::string error;
    };

    class shader_incomplete_error : public shader_error
    {
    public:
        shader_incomplete_error(std::string errorString)
            : shader_error(std::move(errorString)) {}
    };

    class shader_compile_error : public shader_error
    {
    public:
        shader_compile_error(std::string compileError)
            : shader_error(std::move(compileError)) {}
    };

    class shader_link_error : public shader_error
    {
    public:
        shader_link_error(std::string linkError)
            : shader_error(std::move(linkError)) {}
    };

    /**
     * @brief Reference-counted wrapper for OpenGL shader programs
     *
     * Contains and manages an OpenGL shader program. The loaded OpenGL object
     * is reference counted and thus automatically destroyed when it is not
     * referenced by any ShaderPrograms.
     *
     * Use the constrcutor or ShaderProgram::init() to load a new shader
     * program. This decrements the reference count by 1.
     *
     * Call ShaderProgram::bind() to bind the program to the current OpenGL
     * state. This uses glUseProgram() internally.
     */
    class ShaderProgram
    {
    public:
        /**
         * @brief Just create the object, don't load a program
         */
        ShaderProgram() = default;

        /**
         * @brief Load a compute shader program from shader source code
         *
         * @param const std::string& comp The source code of the compute shader
         */
        explicit ShaderProgram(const std::string& comp);

        /**
         * @brief Load a program from shader sources
         *
         * @param const std::string& vert The vertex shader source. Not optional.
         * @param const std::string& frag The fragment shader source. Technically optional, however,
         *                                it is advised to always provide a fragment shader.
         * @param const std::string& tesc The tessellation control shader source. Optional. If this is
         *                                specified, the evaluation shader must be specified as well.
         * @param const std::string& tese The tessellation evaluation shader source. Optional.
         * @param const std::string& geom The geometry shader source. Optional.
         *
         * @throw shader_error
         */
        ShaderProgram(
            const std::string& vert,
            const std::string& frag,
            const std::string& tesc = "",
            const std::string& tese = "",
            const std::string& geom = ""
        );

        /**
         * @brief Get the OpenGL program handle
         *
         * @return GLuint The OpenGL handle to the shader program
         */
        [[nodiscard]]
        auto getProgramID() const -> GLuint;

        /**
         * @brief Load a new shader program
         *
         * Creates a shader program from the specified shader source code files.
         *
         * @param const std::string& vert The vertex shader source. Not optional.
         * @param const std::string& frag The fragment shader source. Technically optional, however,
         *                                it is advised to always provide a fragment shader.
         * @param const std::string& tesc The tessellation control shader source. Optional. If this is
         *                                specified, the evaluation shader must be specified as well.
         * @param const std::string& tese The tessellation evaluation shader source. Optional.
         * @param const std::string& geom The geometry shader source. Optional.
         */
        void init(
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath,
            const std::string& tesselationControlShaderPath = "",
            const std::string& tesselationEvaluationShaderPath = "",
            const std::string& geometryShaderPath = ""
        );

        /**
         * @brief Initialize the shader as a compute shader
         *
         * Load the source code of a compute shader and create a compute program.
         *
         * @param const std::string& computeShaderPath Path to the shader code
         */
        void initComputeProgram(const std::string& computeShaderPath);

        /**
         * @brief Bind the program to the current OpenGL state
         */
        void bind() const;

    private:
        glSharedProgram program;
    };
} // namespace glb

#endif