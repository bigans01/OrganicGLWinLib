#pragma once

#ifndef GLUNIFORMREQUEST_H
#define GLUNIFORMREQUEST_H

#include "GLDataType.h"

/* Description: a simple class that contans a GLDataType and a string, to represent a specific type and name
*  that is desired to be found somewhere. Simply used as a container of metadata, to either assist in creating
*  a uniforming, or checking if one exists.

*/

class GLUniformRequest
{
	public:
		GLUniformRequest(GLDataType in_dataType, std::string in_uniformName);
		GLDataType dataType = GLDataType::NONE;
		std::string uniformName;
};

#endif
