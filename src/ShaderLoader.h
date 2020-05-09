#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

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


    namespace internal
    {
        class ShaderCompilerException : public std::exception
        {
        public:
            ShaderCompilerException(std::string errorMessage)
                : message(std::move(errorMessage)) {}

            auto what() const noexcept -> const char* override {
                return message.c_str();
            }

        private:
            std::string message;
        };


        class ShaderPreCompiler
        {
        public:
            /**
             * @brief Apply gl_base features to shader source code
             *
             * Currently only has the #include feature. All #include
             * directives are replaced with the contents of the included
             * file.
             *
             * Paths in the #include can be absolute or relative to the
             * including shader's directory.
             *
             * @param std::string& code The shader code. Modified in-place.
             * @param const fs::path& shaderPath Path to the shader source.
             *                                   Used for include resolution
             */
            void processShaderCode(std::string& code, const fs::path& shaderPath) noexcept;

        private:
            /**
             * @brief Parse code for #includes and insert included files
             *
             * Modifies the code in-place.
             */
            void processIncludeDirectives(std::string& code, const fs::path& shaderPath);

            /**
             * @param std::string& code The code to insert the file into.
             *                          Modified in-place.
             * @param const fs::path& includeFile The file to read and
             *                                    insert.
             * @param uint32_t character The index at which the included file
             *                           is inserted.
             */
            void insertFile(std::string& code, const fs::path& includeFile, uint32_t character);
        };
    }
} // namespace glb

#endif
