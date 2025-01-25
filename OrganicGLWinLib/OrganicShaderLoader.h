#pragma once

#ifndef ORGANICSHADERLOADER_H
#define ORGANICSHADERLOADER_H

#include <GL/glew.h>
#include "GLBasicProgramData.h"

class OrganicShaderLoader
{
public:
	static GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

	// Below: create vertex shader/fragment shader program, and store all that metadata for a return value.
	static GLBasicProgramData createBasicShaderProgram(const char* vertex_file_path, const char* fragment_file_path);
	static GLuint LoadComputeShader(const char* vertex_file_path);
};

#endif