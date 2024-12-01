#include "stdafx.h"
#include "MShaderBase.h"

void MShaderBase::setSharedObjectPointers(ImGuiButtonPanelContainer* in_parentButtonPanelContainerRef,
	ImGuiSliderFloatPanelContainer* in_parentSliderPanelContainerRef,
	ImGuiInputTextPanelContainer* in_parentInputPanelContainerRef,
	ShaderMachineFeedback* in_parentMachineFeedbackRef,
	MBindingMap* in_parentBindingMapPtr)
{
	parentButtonPanelContainerPtr = in_parentButtonPanelContainerRef;
	parentSliderPanelContainerPtr = in_parentSliderPanelContainerRef;
	parentInputPanelContainerPtr = in_parentInputPanelContainerRef;
	parentMachineFeedbackPtr = in_parentMachineFeedbackRef;
	parentBindingMapPtr = in_parentBindingMapPtr;
}

std::string MShaderBase::fetchMShaderName()
{
	return mShaderName;
}