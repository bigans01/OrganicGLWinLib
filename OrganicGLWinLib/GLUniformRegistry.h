#pragma once

#ifndef GLUNIFORMREGISTRY_H
#define GLUNIFORMREGISTRY_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "EnclaveKeyDef.h"

class GLUniformRegistry
{
public:
	void insertVec2(std::string in_stringValue, glm::vec2 in_vec2);
	void insertVec3(std::string in_stringValue, glm::vec3 in_vec3);
	void insertMat3(std::string in_stringValue, glm::mat3 in_mat3);
	void insertMat4(std::string in_stringValue, glm::mat4 in_mat4);
	void insertFloat(std::string in_stringValue, float in_float);
	void insertInt(std::string in_stringValue, int in_int);
	void insertEnclaveKey(std::string in_stringValue, EnclaveKeyDef::EnclaveKey in_enclaveKey);

	glm::vec2 getVec2(std::string in_stringValue);
	glm::vec3 getVec3(std::string in_stringValue);
	glm::mat3 getMat3(std::string in_stringValue);
	glm::mat4 getMat4(std::string in_stringValue);
	float getFloat(std::string in_stringValue);
	int getInt(std::string in_stringValue);
	EnclaveKeyDef::EnclaveKey get3DKey(std::string in_stringValue);

	float* getFloatRef(std::string in_stringValue);
private:
	std::unordered_map<std::string, glm::vec2> regVec2;
	std::unordered_map<std::string, glm::vec3> regVec3;
	std::unordered_map<std::string, glm::mat3> regMat3;
	std::unordered_map<std::string, glm::mat4> regMat4;
	std::unordered_map<std::string, float> regFloat;
	std::unordered_map<std::string, int> regInt;
	std::unordered_map<std::string, EnclaveKeyDef::EnclaveKey> regEnclaveKey;



};

#endif