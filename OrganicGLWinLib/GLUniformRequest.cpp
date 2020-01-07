#include "stdafx.h"
#include "GLUniformRequest.h"

GLUniformRequest::GLUniformRequest(GLDataType in_dataType, std::string in_uniformName)
{
	dataType = in_dataType;
	uniformName = in_uniformName;
}