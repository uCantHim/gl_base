#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <string>

#include <GL/glew.h>

#include "Shader.h"

namespace glb
{
    /**
     * @brief Provides utility to read and compile shader code
     */
    class ShaderLoader
    {
    public:
        /**
         * @brief Load shaders and link them to a program
         *
         * If a tessellation control shader is provided but no tessellation
         * evaluation shader (or vice versa), these shaders will not be linked
         * into the program.
         *
         * Throws shader_incomplete_error if no valid vertex shader path is
         * specified. All other shader stages are optional.
         *
         * @throw shader_incomplete_error,
         *        shader_compile_error,
         *        shader_link_error,
         *        std::runtime_error
         */
        static GLuint loadProgram(
            const std::string& vert = "",
            const std::string& frag = "",
            const std::string& tesc = "",
            const std::string& tese = "",
            const std::string& geom = ""
        );

        /**
         * @brief Load a compute shader and link it to a compute program
         *
         * @throw shader_incomplete_error
         *        shader_compile_error
         *        shader_link_error
         *        std::runtime_error
         */
        static GLuint loadComputeProgram(const std::string& compPath);

    private:
        /**
         * @throw shader_compile_error
         *        std::runtime_error
         */
        static GLuint loadShader(const std::string& path, GLuint glShaderEnum);

        /**
         * @throw shader_link_error
         *        std::runtime_error
         */
        static GLuint linkProgram(const std::vector<GLuint>& shaders);
    };
} // namespace glb

#endif