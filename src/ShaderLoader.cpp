#include "ShaderLoader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;



GLuint glb::ShaderLoader::loadProgram(
	const std::string& vert,
	const std::string& frag,
	const std::string& tesc,
	const std::string& tese,
	const std::string& geom)
{
	std::vector<GLuint> shaders;

    // Vertex shader - must be present
    if (!fs::is_regular_file(vert))
        throw shader_incomplete_error("Vertex shader " + vert + " is not a valid file");
    shaders.push_back(loadShader(vert, GL_VERTEX_SHADER));

    // Fragment shader - optional
    if (fs::is_regular_file(frag))
        shaders.push_back(loadShader(frag, GL_FRAGMENT_SHADER));

    // Tessellation shaders
    if (fs::is_regular_file(tesc) && fs::is_regular_file(tese))
    {
        shaders.push_back(loadShader(tesc, GL_TESS_CONTROL_SHADER));
        shaders.push_back(loadShader(tese, GL_TESS_EVALUATION_SHADER));
    }

    // Geometry shader
    if (fs::is_regular_file(geom))
        shaders.push_back(loadShader(geom, GL_GEOMETRY_SHADER));

    // Create and link program
    return linkProgram(shaders);
}

GLuint glb::ShaderLoader::loadComputeProgram(const std::string& compPath)
{
    if (!fs::is_regular_file(compPath))
        throw shader_incomplete_error("Compute shader " + compPath + " is not a valid file");

    GLuint shader = loadShader(compPath, GL_COMPUTE_SHADER);

    return linkProgram({ shader });
}

GLuint glb::ShaderLoader::loadShader(const std::string& path, GLuint glShaderEnum)
{
	// Read code from file
	std::string shaderCode;

	std::ifstream file(path);
	if (file.is_open())
	{
		std::stringstream code;
		code << file.rdbuf();
		shaderCode = code.str();
		file.close();
	}
    else
        throw std::runtime_error("Only pass valid filepaths to ShaderLoader::loadShader()!");

	// Create and compile shader
	GLuint shader = glCreateShader(glShaderEnum);
	const char* code = shaderCode.c_str(); // Is required because GL needs a double pointer
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);

	GLint infoLogLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
        std::vector<char> infoLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());
        throw shader_compile_error(std::string(infoLog.data()));
	}

	return shader;
}

GLuint glb::ShaderLoader::linkProgram(const std::vector<GLuint>& shaders)
{
	GLuint program = glCreateProgram();

	for (auto shader : shaders)
	{
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
	glLinkProgram(program);

	GLint infoLogLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
        std::vector<char> infoLog(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
        throw shader_link_error(std::string(infoLog.data()));
	}

	return program;
}