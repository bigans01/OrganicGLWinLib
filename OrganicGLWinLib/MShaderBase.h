#pragma once

#ifndef MSHADERBASE_H
#define MSHADERBASE_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "GLUniformRegistry.h"

#include <cmath>
#include "ImGuiButtonPanelContainer.h"
#include "ImGuiSliderFloatPanelContainer.h"
#include "ImGuiInputTextPanelContainer.h"
#include "ShaderMachineFeedback.h"
#include "MAPIObjectManager.h"
#include "MAPIObjectRequest.h"

#include "MGearManager.h"
#include "MAPIObjectUpdate.h"


/*

Description: base class that serves as the main parent for MShader classes, 
such as MSBasicCompute.

*/

class MShaderBase
{
	public:
		// Below: should be called immediately after a child of this class is instantiated
		void setSharedObjectPointers(ImGuiButtonPanelContainer* in_parentButtonPanelContainerPtr,
			ImGuiSliderFloatPanelContainer* in_parentSliderPanelContainerPtr,
			ImGuiInputTextPanelContainer* in_parentInputPanelContainerPtr,
			ShaderMachineFeedback* in_parentMachineFeedbackPtr,
			MAPIObjectManager* in_parentBindingMapPtr,
			GLUniformRegistry* in_parentValueRegistryPtr
		);

		std::vector<MAPIObjectRequest> fetchMShaderBindingRequests();	// should be called after 

		std::string fetchMShaderName();	
		GLUniformRegistry* getLocalValueRegistryRef();	// get a ref to mShaderLocalValueRegistry;
														// can be used by MShaderController or other classes to check for existence
														// of certain uniforms, or other similiar operations.

		void cleanupSpawnedMGears();			// call cleanupMGears() on all gears in the mShaderGearManager

		void setBindingsForGears();		// ultimately used to call setObjectBindings() for all MGear objects found in the MGearManager (mShaderGearManager);
										// should be called only after objects required by MGear objects have been instantiated/setup.

		bool checkForRequiredGearObjects(std::queue<Message>* in_messageQueueRef);		// calls the verifyRequiredGearObjects function on mShaderGearManager;
																						// return value should be true if all requested objects were found in the referenced MAPIObjectManager.


		// ***** Required virtual functions *****
		// Below: should be called after setSharedObjectPointers above
		virtual void setupMShaderRequestsAndName() = 0;		// set the shader name, create MAPIObjectRequest objects for expected bindings (bindingRequests), 
															// required uniform registry variables, etc.
															
		virtual Message checkRequiredBindings() = 0;	// should be called after requested bindings have been sent to an instance that is a child of this class;
														// it should analyze the contents of the referenced MAPIObjectManager (parentBindingMapPtr) to see if 
														// the expected bindings are available.
		
		virtual void prepareAndRender() = 0;	// allows the MShader to run any custom code it needs (prepare), before rendering
												// all MGear objects contained in the MAPIObjectManager; this function is also where the logic
												// for checking for creates/deletes in the MAPIObjectManager is carried out.

		void flagForFullScanOnNextTick();	// used to indicate that a full scan of objects in the MAPIObjectManager should be done, in the next call to
											// prepareAndRender(); mainly used when a new MShader is being selected/switched to at run time, or when the engine first starts.

		void flagForUpdateScanOnNextTick(MAPIObjectUpdate in_updatedObject);		// will set scanForUpdatesFlag to true, and signal to all constituent MGear objects
		                                                                            // that they should run a scan against the contents of updatesToProcess; be sure to reset the value
																					// of scanForUpdatesFlag  with a call to resetUpdateObjects;

	protected:
		void renderAllGears();
		void resetUpdateObjects();	// revert the value of scanForUpdatesFlag back to false; should be called in the prepareAndRender
									// function of the children of this class, after the scanning logic is handled upon scanForUpdatesFlag being true.

		// references to shareable objects from the parent MShaderController
		ImGuiButtonPanelContainer* parentButtonPanelContainerPtr = nullptr;
		ImGuiSliderFloatPanelContainer* parentSliderPanelContainerPtr  = nullptr;
		ImGuiInputTextPanelContainer* parentInputPanelContainerPtr = nullptr;
		ShaderMachineFeedback* parentMachineFeedbackPtr = nullptr;
		MAPIObjectManager* parentBindingMapPtr = nullptr;
		GLUniformRegistry* parentValueRegistryPtr = nullptr;

		bool scanForUpdatesFlag = false;	// set to true, when the MShader needs to notify it's gears that they should scan for
											// object updates in the MAPIObjectManager (i.e, was a buffer inserted in it)

		bool runFullScan = false;	// if flagForFullScanOnNextTick is called, set this to true. set to false when resetUpdateObjects is called.

		std::string mShaderName = "";	// must be set by child class

		std::vector<MAPIObjectRequest> bindingRequests;	// a vector that contains the names of requested bindings

		std::vector<MAPIObjectUpdate> updatesToProcess;	// contains updates (creates, or deletes) that should be processed; 
														// make sure all gears get an opportunity to scan this before its cleared by 

		GLUniformRegistry mShaderLocalValueRegistry;	// will store the "preferred" uniform/local values that the
														// children of this class will want; each child class must manually
														// define these.

		MGearManager mShaderGearManager;	// controls all MGear objects that the MShaderBase children classes will utilize
};

#endif
