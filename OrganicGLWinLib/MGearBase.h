#pragma once

#ifndef MGEARBASE_H
#define MGEARBASE_H

#include "GLUniformRegistry.h"
#include "MAPIObjectManager.h"

/*
* 
* Description: This class serves as the base class for MGear-derived classes; the MGearBase class includes references to
* the MAPIObjectManager and GLUniformRegistry that ultimately belongs to the MShaderController class, so that derivatives of 
* this class may directly reference those objects to get metadata for rendering.
* 
* MGearBase children classes are directly responsible for rendering, using the referenced objects above; for instance, an MGear
* will need to utilize the GLUniformRegistry to acquire specific uniform values (such as a vec3), and use the MAPIObjectManager to 
* find buffers, framebuffers, etc. These can all be referenced before rendering.

*/

class MGearBase
{
	public:
		virtual void render() = 0;	// should be called every tick in which this MGear is run

		// Below: always call this first after the derived class is instantiated into a map
		void setMGearPointers(MAPIObjectManager* in_mGearObjectManagerRef,
			GLUniformRegistry* in_mGearUniformRegistryRef);
	private:
		MAPIObjectManager* mGearObjectManagerRef = nullptr;
		GLUniformRegistry* mGearUniformRegistryRef = nullptr;
};

#endif
