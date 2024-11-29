#include "stdafx.h"
#include "MBindingMap.h"

/*

Description: maps strings to OpenGL binding IDs; can be used by any OpenGL object that 
utilizes bindings -- VAO, buffers, textures. It's purpose is to universally store all these IDs 
in maps that can be shared across multiple MShaderBase instances.

*/

bool MBindingMap::doesBindingExist(std::string in_bindingToFind)
{
	bool wasFound = false;
	auto bindingFinder = bindings.find(in_bindingToFind);
	if (bindingFinder != bindings.end())
	{
		wasFound = true;
	}
	return wasFound;
}
																
void MBindingMap::insertBinding(std::string in_bindingName, int in_bindingValue)
{
	bindings[in_bindingName] = in_bindingValue;
}