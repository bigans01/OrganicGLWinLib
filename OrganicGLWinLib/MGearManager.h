#pragma once

#ifndef MGEARMANAGER_H
#define MGEARMANAGER_H

#include "MGearBase.h"
#include "GLUniformRegistry.h"
#include "MAPIObjectManager.h"

/*
*	Description: This class serves as a container of MGear-derived classes, and simplifies 
*	rendering by using a single function call the render() virtual function on all MGear objects.
*	Specific MGear objects can be checked for, and this class can also fetch pointers to specific
*	MGear objects that exist.
*/

class MGearManager
{
	public:
		// Below: always call this initialization function before anything else
		void initializeMGearManager(MAPIObjectManager* in_msbObjectManagerRef,
									GLUniformRegistry* in_msbUniformRegistrRef);

		bool insertMGear(std::string in_mGearName, std::unique_ptr<MGearBase> in_newMGear);	// insert a specific MGear:
																							// use std::move when setting up the parameters
																							// for this function call; returns true
																							// if it was actually inserted.

		bool isMGearInManager(std::string in_mGearName);	// returns true if an MGear exists
		MGearBase* getMGearRef(std::string in_mMGearName);	// returns a pointer to an MGearBase-derived object; the value will be null
															// if it doesn't exist.

		void renderMGears();	// call render() on all MGear objects in the mGearMap.
	private:
		std::unordered_map<std::string, std::unique_ptr<MGearBase>> mGearMap;	// contains all MGear objects that will be utilized for rendering.

		MAPIObjectManager* msbObjectManagerRef = nullptr;
		GLUniformRegistry* msbUniformRegistryRef = nullptr;
};

#endif
