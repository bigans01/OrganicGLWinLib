#pragma once

#ifndef GLUNIFORMREQUEST_H
#define GLUNIFORMREQUEST_H

#include "GLDataType.h"
#include <iostream>

class GLUniformRequest
{
	public:
		GLUniformRequest(GLDataType in_dataType, std::string in_uniformName);
		GLDataType dataType = GLDataType::NONE;
		std::string uniformName;
};

#endif
