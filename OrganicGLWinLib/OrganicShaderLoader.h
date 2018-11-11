#pragma once

#ifndef ORGANICSHADERLOADER_H
#define ORGANICSHADERLOADER_H

#include <GL/glew.h>

class OrganicShaderLoader
{
public:
	static GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};

#endif