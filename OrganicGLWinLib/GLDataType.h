#pragma once

#ifndef GLDATATYPE_H
#define GLDATATYPE_H

/*

Description:

This enum class is to used to represent a type of uniform data
that can reside in the GLUniformRegistry class. It is primarily in 
GLUniformRequest objects, in order to determine the kind of data 
a Gear may request. It is also used in Gear::sendUniform to 
send data back to the Gear requesting it, via a templated function.

See usage in:

-Gear::sendUniform
-ShaderMachineBase::sendGearUniforms

*/

enum class GLDataType
{
	NONE,
	VEC2,	// glm::vec2
	VEC3,	// glm::vec3
	MAT3,	// glm::mat3
	MAT4,	// glm::mat4
	FLOAT,	// float
	INT,	// int
	KEY3D	// EnclaveKeyDef::EnclaveKey (the x/y/z will be separate int values in the shader)
};

#endif
