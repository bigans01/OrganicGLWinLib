#include "stdafx.h"
#include "MShaderBase.h"

void MShaderBase::setSharedObjectPointers(ImGuiButtonPanelContainer* in_parentButtonPanelContainerRef,
	ImGuiSliderFloatPanelContainer* in_parentSliderPanelContainerRef,
	ImGuiInputTextPanelContainer* in_parentInputPanelContainerRef,
	ShaderMachineFeedback* in_parentMachineFeedbackRef,
	MAPIObjectManager* in_parentBindingMapPtr,
	GLUniformRegistry* in_parentValueRegistryPtr
)
{
	parentButtonPanelContainerPtr = in_parentButtonPanelContainerRef;
	parentSliderPanelContainerPtr = in_parentSliderPanelContainerRef;
	parentInputPanelContainerPtr = in_parentInputPanelContainerRef;
	parentMachineFeedbackPtr = in_parentMachineFeedbackRef;
	parentBindingMapPtr = in_parentBindingMapPtr;
	parentValueRegistryPtr = in_parentValueRegistryPtr;
}

std::vector<MAPIObjectRequest> MShaderBase::fetchMShaderBindingRequests()
{
	return bindingRequests;
}

std::string MShaderBase::fetchMShaderName()
{
	return mShaderName;
}

GLUniformRegistry* MShaderBase::getLocalValueRegistryRef()
{
	return &mShaderLocalValueRegistry;
}

void MShaderBase::cleanupSpawnedMGears()
{
	mShaderGearManager.cleanupMGears();
}

void MShaderBase::setBindingsForGears()
{
	mShaderGearManager.createRequiredGearObjects();
}

bool MShaderBase::checkForRequiredGearObjects(std::queue<Message>* in_messageQueueRef)
{
	return mShaderGearManager.verifyRequiredGearObjects(in_messageQueueRef);
}

void MShaderBase::flagForUpdateScanOnNextTick(MAPIObjectUpdate in_updatedObject)
{
	scanForUpdatesFlag = true;
	updatesToProcess.push_back(in_updatedObject);
}

void MShaderBase::flagForFullScanOnNextTick()
{
	scanForUpdatesFlag = true;
	runFullScan = true;
}

void MShaderBase::resetUpdateObjects()
{
	scanForUpdatesFlag = false;
	runFullScan = false;
	updatesToProcess.clear();
}

void MShaderBase::renderAllGears()
{
	mShaderGearManager.renderMGears();
}