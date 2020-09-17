#include "stdafx.h"
#include "GLUniformRegistry.h"

void GLUniformRegistry::insertVec2(std::string in_stringValue, glm::vec2 in_vec2) 
{
	regVec2[in_stringValue] = in_vec2;
}

void GLUniformRegistry::insertVec3(std::string in_stringValue, glm::vec3 in_vec3) 
{
	regVec3[in_stringValue] = in_vec3;
}

void GLUniformRegistry::insertMat3(std::string in_stringValue, glm::mat3 in_mat3)
{
	regMat3[in_stringValue] = in_mat3;
}

void GLUniformRegistry::insertMat4(std::string in_stringValue, glm::mat4 in_mat4) 
{
	regMat4[in_stringValue] = in_mat4;
}

void GLUniformRegistry::insertFloat(std::string in_stringValue, float in_float)
{
	regFloat[in_stringValue] = in_float;
}

void GLUniformRegistry::insertInt(std::string in_stringValue, int in_int)
{
	regInt[in_stringValue] = in_int;
}

glm::vec2 GLUniformRegistry::getVec2(std::string in_stringValue)
{
	return regVec2[in_stringValue];
}

glm::vec3 GLUniformRegistry::getVec3(std::string in_stringValue)
{
	return regVec3[in_stringValue];
}

glm::mat3 GLUniformRegistry::getMat3(std::string in_stringValue)
{
	return regMat3[in_stringValue];
}

glm::mat4 GLUniformRegistry::getMat4(std::string in_stringValue)
{
	return regMat4[in_stringValue];
}

float GLUniformRegistry::getFloat(std::string in_stringValue)
{
	return regFloat[in_stringValue];
}

int GLUniformRegistry::getInt(std::string in_stringValue)
{
	return regInt[in_stringValue];
}

float* GLUniformRegistry::getFloatRef(std::string in_stringValue)
{
	return &regFloat[in_stringValue];
}

/*
void GLUniformRegistry::processGearUniformRequests(Gear* in_gear)
{
	auto requestVectorBegin = in_gear->getUniformRequests()->begin();
	auto requestVectorEnd = in_gear->getUniformRequests()->end();
	for (requestVectorBegin; requestVectorBegin != requestVectorEnd; requestVectorBegin++)
	{
		if (requestVectorBegin->dataType == GLDataType::VEC2)
		{

		}
		else if (requestVectorBegin->dataType == GLDataType::VEC3)
		{

		}
		else if (requestVectorBegin->dataType == GLDataType::MAT4)
		{
			//glm::mat4 outMat4 = regMat4[requestVectorBegin->uniformName];
			//in_gear->sendUniformMat4(requestVectorBegin->uniformName, outMat4);
		}
		else if (requestVectorBegin->dataType == GLDataType::FLOAT)
		{

		}
	}
}
*/