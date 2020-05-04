#include "Shader.h"

#include "ShaderLoader.h"



glb::ShaderProgram::ShaderProgram(const std::string& comp)
{
    initComputeProgram(comp);
}

glb::ShaderProgram::ShaderProgram(
    const std::string& vert,
    const std::string& frag,
    const std::string& tesc,
    const std::string& tese,
    const std::string& geom)
{
    init(vert, frag, tesc, tese, geom);
}

GLuint glb::ShaderProgram::getProgramID() const
{
	return *program;
}

void glb::ShaderProgram::init(
	const std::string& vertexShaderPath,
	const std::string& fragmentShaderPath,
	const std::string& tesselationControlShaderPath,
	const std::string& tesselationEvaluationShaderPath,
	const std::string& geometryShaderPath)
{
	program.release();

	program = ShaderLoader::loadProgram(
		vertexShaderPath,
        fragmentShaderPath,
		tesselationControlShaderPath,
        tesselationEvaluationShaderPath,
		geometryShaderPath
	);
}

void glb::ShaderProgram::initComputeProgram(const std::string& computeShaderPath)
{
	program.release();

	program = ShaderLoader::loadComputeProgram(computeShaderPath);
}

void glb::ShaderProgram::bind() const
{
	glUseProgram(*program);
}
