#pragma once

#ifndef MSHADERCATALOG_H
#define MSHADERCATALOG_H

#include "MShaderBase.h"

/*

Description: This class intended to store and manage all usable instances of MShaderBase-derived classes that
the MShaderController class might instantiate. Direct access to the objects by the unique_ptr objects is done
via a call to getShaderRef, 

*/

class MShaderCatalog
{
	public:
		bool insertMShader(std::string in_shaderName, std::unique_ptr<MShaderBase> in_newShaderInstancePtr);	// attempts to insert a shader into the catalog; returns true if this is the case.
		bool isShaderInMap(std::string in_shaderName);	// returns true if the shader given by the string exists.
		MShaderBase* getShaderRef(std::string in_shaderName);	// returns a pointer to the specified shader, mapped in the given string;
																// if the shader wasn't found, returns a nullptr.
	private:
		std::unordered_map<std::string, std::unique_ptr<MShaderBase>> shaderMap;	// should contain all MShaderBase-derived classes that should be used

};

#endif
