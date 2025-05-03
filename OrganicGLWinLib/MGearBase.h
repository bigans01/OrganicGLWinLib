#pragma once

#ifndef MGEARBASE_H
#define MGEARBASE_H

#include "GLUniformRegistry.h"
#include "MAPIObjectManager.h"
#include "OrganicShaderLoader.h"
#include "MAPIObjectMetadata.h"
#include "MAPIObjectUpdate.h"

/*
* 
* Description: This class serves as the base class for MGear-derived classes; the MGearBase class includes references to
* the MAPIObjectManager and GLUniformRegistry that ultimately belongs to the MShaderController class, so that derivatives of 
* this class may directly reference those objects to get metadata for rendering.
* 
* MGearBase children classes are directly responsible for rendering, using the referenced objects above; for instance, an MGear
* will need to utilize the GLUniformRegistry to acquire specific uniform values (such as a vec3), and use the MAPIObjectManager to 
* find buffers, framebuffers, etc. These can all be referenced before rendering.
* 
* The MGear children classes should also be responsible for creating their own shader programs; this will simplify the management of the creation
* of the shaders, whereas in the old model it was all managed through calls to functions such as OrganicGLWinUtils::loadShadersViaMode


*/

class MGearBase
{
	public:
		// required virtual functions
		virtual void initializeMGear() = 0;		//	set up varrious things required by the MGear; should be called immediately after
												//	setMGearPointers

		virtual std::vector<MAPIObjectMetadata> fetchRequiredObjects() = 0;	// used to verify that objects are instantiated/setup correctly;
																			// called by MGearManager::verifyRequiredGearObjects()

		virtual void render() = 0;	// should be called every tick in which this MGear is run; ensure that switchToMGearProgram() is called first in this function

		virtual void cleanupMGear() = 0;	// should be called before MGear is deleted (deletes shaders, programs, other resources)

		virtual void setObjectBindings() = 0;	// use to set up VAOs, other binding objects; ensure that the resources exist before calling
												// this function.

		virtual void conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef) = 0;	// this function is used by the MGearBase-child class to signal that it 
																															// should either do a full scan of all objects, or perform an individual scan
																															// of all objects in the referenced vector.


		// Below: always call this first after the derived class is instantiated into a map
		void setMGearPointers(MAPIObjectManager* in_mGearObjectManagerRef,
							GLUniformRegistry* in_mGearUniformRegistryRef);

		void switchToMGearProgram();	// should always be called first in the render() virtual function for 
										// a given MGearBase-derived class/object.
	protected:
		MAPIObjectManager* mGearObjectManagerRef = nullptr;
		GLUniformRegistry* mGearUniformRegistryRef = nullptr;

		int mGearProgramID = 0;	// used to store the program ID that is returned when compiling compute/fragment/vertex shaders, via functions 
								// such as OrganicShaderLoader::LoadShaders; this value should be set when the shaders are loaded in the call to initializeMGear
};

#endif
