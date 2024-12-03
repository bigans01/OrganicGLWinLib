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

void GLUniformRegistry::insertEnclaveKey(std::string in_stringValue, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	regEnclaveKey[in_stringValue] = in_enclaveKey;
}

glm::vec2 GLUniformRegistry::getVec2(std::string in_stringValue)
{
	glm::vec2 returnVec;
	auto vec2Finder = regVec2.find(in_stringValue);
	if (vec2Finder != regVec2.end())
	{
		returnVec = regVec2[in_stringValue];
	}
	return returnVec;
}

bool GLUniformRegistry::doesVec2Exist(std::string in_stringValue)
{
	bool vec2Exists = false;
	auto vec2Finder = regVec2.find(in_stringValue);
	if (vec2Finder != regVec2.end())	// it exists
	{
		vec2Exists = true;
	}
	return vec2Exists;
}

glm::vec3 GLUniformRegistry::getVec3(std::string in_stringValue)
{
	glm::vec3 returnVec;
	auto vec3Finder = regVec3.find(in_stringValue);
	if (vec3Finder != regVec3.end())
	{
		returnVec = regVec3[in_stringValue];
	}
	return returnVec;
}

bool GLUniformRegistry::doesVec3Exist(std::string in_stringValue)
{
	bool vec3Exists = false;
	auto vec3Finder = regVec3.find(in_stringValue);
	if (vec3Finder != regVec3.end())
	{
		vec3Exists = true;
	}
	return vec3Exists;
}

glm::mat3 GLUniformRegistry::getMat3(std::string in_stringValue)
{
	glm::mat3 returnMat3;
	auto mat3Finder = regMat3.find(in_stringValue);
	if (mat3Finder != regMat3.end())
	{
		returnMat3 = regMat3[in_stringValue];
	}
	return returnMat3;
}

bool GLUniformRegistry::doesMat3Exist(std::string in_stringValue)
{
	bool mat3Exists = false;
	auto mat3Finder = regMat3.find(in_stringValue);
	if (mat3Finder != regMat3.end())
	{
		mat3Exists = true;
	}
	return mat3Exists;
}

glm::mat4 GLUniformRegistry::getMat4(std::string in_stringValue)
{
	glm::mat4 returnMat4;
	auto mat4Finder = regMat4.find(in_stringValue);
	if (mat4Finder != regMat4.end())
	{
		returnMat4 = regMat4[in_stringValue];
	}
	return returnMat4;
}

bool GLUniformRegistry::doesMat4Exist(std::string in_stringValue)
{
	bool mat4Exists = false;
	auto mat4Finder = regMat4.find(in_stringValue);
	if (mat4Finder != regMat4.end())
	{
		mat4Exists = true;
	}
	return mat4Exists;
}

float GLUniformRegistry::getFloat(std::string in_stringValue)
{
	float returnFloat = 0.0f;
	auto floatFinder = regFloat.find(in_stringValue);
	if (floatFinder != regFloat.end())
	{
		returnFloat = regFloat[in_stringValue];
	}
	return returnFloat;
}

bool GLUniformRegistry::doesFloatExist(std::string in_stringValue)
{
	bool floatExists = false;
	auto intFinder = regFloat.find(in_stringValue);
	if (intFinder != regFloat.end())
	{
		floatExists = true;
	}
	return floatExists;
}


int GLUniformRegistry::getInt(std::string in_stringValue)
{
	int returnInt = 0;
	auto intFinder = regInt.find(in_stringValue);
	if (intFinder != regInt.end())
	{
		returnInt = regInt[in_stringValue];
	}
	return returnInt;
}

bool GLUniformRegistry::doesIntExist(std::string in_stringValue)
{
	bool intExists = false;
	auto intFinder = regInt.find(in_stringValue);
	if (intFinder != regInt.end())
	{
		intExists = true;
	}
	return intExists;
}

EnclaveKeyDef::EnclaveKey GLUniformRegistry::get3DKey(std::string in_stringValue)
{
	EnclaveKeyDef::EnclaveKey returnKey;
	auto keyFinder = regEnclaveKey.find(in_stringValue);
	if (keyFinder != regEnclaveKey.end())
	{
		returnKey = regEnclaveKey[in_stringValue];
	}
	return returnKey;
}

bool GLUniformRegistry::does3DKeyExist(std::string in_stringValue)
{
	bool keyExists = false;
	auto keyFinder = regEnclaveKey.find(in_stringValue);
	if (keyFinder != regEnclaveKey.end())
	{
		keyExists = true;
	}
	return keyExists;
}

float* GLUniformRegistry::getFloatRef(std::string in_stringValue)
{
	return &regFloat[in_stringValue];
}
