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
#include "MBindingMap.h"
//#include "GLUniformRegistry.h"

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
			MBindingMap* in_parentBindingMapPtr,
			GLUniformRegistry* in_parentValueRegistryPtr
		);

		std::string fetchMShaderName();	
		GLUniformRegistry* getLocalValueRegistryRef();	// get a ref to mShaderLocalValueRegistry;
														// can be used by MShaderController or other classes to check for existence
														// of certain uniforms, or other similiar operations.

		// Below: should be called after setSharedObjectPointers above
		virtual void setupMShaderRequestsAndName() = 0;		// create strings for expected bindings (bindingRequests), etc
		virtual Message checkRequiredBindings() = 0;	// should be called after requested bindings have been sent to an instance that is a child of this class;
														// it should analyze the contents of the referenced MBindingMap (parentBindingMapPtr) to see if 
														// the expected bindings are available.

	protected:
		// references to shareable objects from the parent MShaderController
		ImGuiButtonPanelContainer* parentButtonPanelContainerPtr = nullptr;
		ImGuiSliderFloatPanelContainer* parentSliderPanelContainerPtr  = nullptr;
		ImGuiInputTextPanelContainer* parentInputPanelContainerPtr = nullptr;
		ShaderMachineFeedback* parentMachineFeedbackPtr = nullptr;
		MBindingMap* parentBindingMapPtr = nullptr;
		GLUniformRegistry* parentValueRegistryPtr = nullptr;

		std::string mShaderName = "";	// must be set by child class

		std::vector<std::string> bindingRequests;	// a vector that contains the names of requested bindings

		GLUniformRegistry mShaderLocalValueRegistry;	// will store the "preferred" uniform/local values that the
														// children of this class will want; each child class must manually
														// define these.
};

#endif
