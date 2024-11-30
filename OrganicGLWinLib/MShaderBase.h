#pragma once

#ifndef MSHADERBASE_H
#define MSHADERBASE_H

#include <cmath>
#include "ImGuiButtonPanelContainer.h"
#include "ImGuiSliderFloatPanelContainer.h"
#include "ImGuiInputTextPanelContainer.h"
#include "ShaderMachineFeedback.h"
#include "MBindingMap.h"

class MShaderBase
{
	public:
		// Below: should be called immediately after a child of this class is instantiated
		void setSharedObjectPointers(ImGuiButtonPanelContainer* in_parentButtonPanelContainerPtr,
			ImGuiSliderFloatPanelContainer* in_parentSliderPanelContainerPtr,
			ImGuiInputTextPanelContainer* in_parentInputPanelContainerPtr,
			ShaderMachineFeedback* in_parentMachineFeedbackPtr,
			MBindingMap* in_parentBindingMapPtr);

		// Below: should be called after setSharedObjectPointers above
		virtual void setupMShaderRequests() = 0;		// create strings for expected bindings (bindingRequests), etc
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

		std::vector<std::string> bindingRequests;	// a vector that contains the names of requested bindings
};

#endif
