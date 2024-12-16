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
	// insertion functions
	void insertVec2(std::string in_stringValue, glm::vec2 in_vec2);
	void insertVec3(std::string in_stringValue, glm::vec3 in_vec3);
	void insertMat3(std::string in_stringValue, glm::mat3 in_mat3);
	void insertMat4(std::string in_stringValue, glm::mat4 in_mat4);
	void insertFloat(std::string in_stringValue, float in_float);
	void insertInt(std::string in_stringValue, int in_int);
	void insertEnclaveKey(std::string in_stringValue, EnclaveKeyDef::EnclaveKey in_enclaveKey);

	// deletion functions
	void deleteVec2(std::string in_stringValue);
	void deleteVec3(std::string in_stringValue);
	void deleteMat3(std::string in_stringValue);
	void deleteMat4(std::string in_stringValue);
	void deleteFloat(std::string in_stringValue);
	void deleteInt(std::string in_stringValue);
	void deleteEnclaveKey(std::string in_stringValue);


	// retrieval functions
	glm::vec2 getVec2(std::string in_stringValue);
	glm::vec3 getVec3(std::string in_stringValue);
	glm::mat3 getMat3(std::string in_stringValue);
	glm::mat4 getMat4(std::string in_stringValue);
	float getFloat(std::string in_stringValue);
	int getInt(std::string in_stringValue);
	EnclaveKeyDef::EnclaveKey get3DKey(std::string in_stringValue);

	// existence-checking functions
	bool doesVec2Exist(std::string in_stringValue);
	bool doesVec3Exist(std::string in_stringValue);
	bool doesMat3Exist(std::string in_stringValue);
	bool doesMat4Exist(std::string in_stringValue);
	bool doesFloatExist(std::string in_stringValue);
	bool doesIntExist(std::string in_stringValue);
	bool does3DKeyExist(std::string in_stringValue);

	float* getFloatRef(std::string in_stringValue);

	void removeCommonValues(GLUniformRegistry* in_otherRegistryRef);	// remove values from this registry object, 
																		// that are found in the referenced one (does not remove the value from referenced registry)

	// name-retrieval functions
	std::vector<std::string> fetchRegVec2Names();
	std::vector<std::string> fetchRegVec3Names();
	std::vector<std::string> fetchRegMat3Names();
	std::vector<std::string> fetchRegMat4Names();
	std::vector<std::string> fetchRegFloatNames();
	std::vector<std::string> fetchRegIntNames();
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