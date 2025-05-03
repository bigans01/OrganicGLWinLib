#include "stdafx.h"
#include "MShaderSelectionCycler.h"

MShaderSelectionCycler::MShaderSelectionCycler()
{
	cycleArray[0] = nullptr;
	cycleArray[1] = nullptr;
}

void MShaderSelectionCycler::setRefs(MShaderCatalog* in_cyclerCatalogRef,
									GLUniformRegistry* in_controllerRegistryRef,
									MGCIndex* in_controllerMGCIndexRef)
{
	cyclerCatalogRef = in_cyclerCatalogRef;
	cyclerControllerRegistryRef = in_controllerRegistryRef;
	cyclerControllerMGCIndexRef = in_controllerMGCIndexRef;
}

Message MShaderSelectionCycler::switchToMShader(std::string in_shaderName)
{
	int switchSuccessful = 0;
	std::string switchMessage = "";

	// only set the cycleArray[0] value if the shader actually exists.
	if (cyclerCatalogRef->isShaderInMap(in_shaderName))
	{
		// Below: used only when a single shader has been selected.
		if (currentLoadedShaders == 0)
		{
			cycleArray[0] = cyclerCatalogRef->getShaderRef(in_shaderName);
			cycleArray[0]->flagForFullScanOnNextTick();
			iterateLoadedShaders();

			// found shader OK
			switchSuccessful = 1;
			switchMessage = "Switched to MShader, " + in_shaderName + ".";
		}

		// Below: used when a new shader needs to be inserted, after we've inserted exactly one previous to this call.
		else if (currentLoadedShaders >= 1)
		{
			// only transition if it isn't the same shader.
			if (!(cycleArray[0]->fetchMShaderName() == cyclerCatalogRef->getShaderRef(in_shaderName)->fetchMShaderName()))
			{
				MShaderBase* tempPtr = cycleArray[0];


				// swap the shader positions.
				cycleArray[1] = tempPtr;
				cycleArray[0] = cyclerCatalogRef->getShaderRef(in_shaderName);
				cycleArray[0]->flagForFullScanOnNextTick();

				// gather the soon-to-be old shader's preferred uniforms; erase them from the MShaderController's registry.
				auto oldMShaderPreferredValues = tempPtr->getLocalValueRegistryRef();

				// values in the old shader, but not in the new shader, should be removed from the parent MShaderController's registry.
				removeObsoleteValues();

				// get the preferred uniforms from the new shader; apply these uniforms only if the gradients don't exist yet,
				// as a gradient existing means that it is already operating on a value in the controller's registry. This will
				// effectively update the controller's registry with the values from the new shader, as long as they didn't exist yet already.
				updateControllerRegistryForNonexistentGradients(cycleArray[0]->getLocalValueRegistryRef());


				iterateLoadedShaders();

				// found shader OK
				switchSuccessful = 1;
				switchMessage = "Switched to MShader, " + in_shaderName + ".";
			}
			else
			{
				switchMessage = "Switch to MShader, " + in_shaderName + " failed, as it is currently in use.";
			}
		}
	}

	// shader wasn't in catalog
	else
	{
		switchMessage = in_shaderName + " not found in catalog! ";
	}

	return Message(MessageType::MSHADERSELECTIONCYCLER_ATTEMPT, switchSuccessful, switchMessage);
}

void MShaderSelectionCycler::updateControllerRegistryForNonexistentGradients(GLUniformRegistry* in_newShaderRegistryRef)
{
	// check vec2
	auto fetchedVec2Strings = in_newShaderRegistryRef->fetchRegVec2Names();
	for (auto& fetchedVec2StringsIter : fetchedVec2Strings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as vec2,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedVec2StringsIter))
		{
			cyclerControllerRegistryRef->insertVec2(fetchedVec2StringsIter, in_newShaderRegistryRef->getVec2(fetchedVec2StringsIter));
		}
	}

	// check vec3 
	auto fetchedVec3Strings = in_newShaderRegistryRef->fetchRegVec3Names();
	for (auto& fetchedVec3StringsIter : fetchedVec3Strings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as vec3,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedVec3StringsIter))
		{
			cyclerControllerRegistryRef->insertVec3(fetchedVec3StringsIter, in_newShaderRegistryRef->getVec3(fetchedVec3StringsIter));
		}
	}

	// check mat3
	auto fetchedMat3Strings = in_newShaderRegistryRef->fetchRegMat3Names();
	for (auto& fetchedMat3StringsIter : fetchedMat3Strings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as mat3,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedMat3StringsIter))
		{
			cyclerControllerRegistryRef->insertMat3(fetchedMat3StringsIter, in_newShaderRegistryRef->getMat3(fetchedMat3StringsIter));
		}
	}

	// check mat4
	auto fetchedMat4Strings = in_newShaderRegistryRef->fetchRegMat4Names();
	for (auto& fetchedMat4StringsIter : fetchedMat4Strings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as mat4,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedMat4StringsIter))
		{
			cyclerControllerRegistryRef->insertMat4(fetchedMat4StringsIter, in_newShaderRegistryRef->getMat4(fetchedMat4StringsIter));
		}
	}

	// check float
	auto fetchedFloatStrings = in_newShaderRegistryRef->fetchRegFloatNames();
	for (auto& fetchedFloatStringsIter : fetchedFloatStrings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as float,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedFloatStringsIter))
		{
			cyclerControllerRegistryRef->insertFloat(fetchedFloatStringsIter, in_newShaderRegistryRef->getFloat(fetchedFloatStringsIter));
		}
	}
	
	// check int
	auto fetchedIntStrings = in_newShaderRegistryRef->fetchRegIntNames();
	for (auto& fetchedIntStringsIter : fetchedIntStrings)
	{
		// as long as it doesn't already exist as a gradient, insert the value as int,
		// into the controllers uniform registry.
		if (!cyclerControllerMGCIndexRef->doesGradientExist(fetchedIntStringsIter))
		{
			cyclerControllerRegistryRef->insertInt(fetchedIntStringsIter, in_newShaderRegistryRef->getInt(fetchedIntStringsIter));
		}
	}
}

void MShaderSelectionCycler::removeObsoleteValues()
{
	// old/new shader pointers
	auto oldShaderPtr = getPreviousShaderRef();
	auto newShaderPtr = getTargetShaderRef();

	// check vec2
	for (auto& currentOldVec2 : oldShaderPtr->getLocalValueRegistryRef()->fetchRegVec2Names())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesVec2Exist(currentOldVec2))
		{
			cyclerControllerRegistryRef->deleteVec2(currentOldVec2);
		}
	}

	// check vec3
	for (auto& currentOldVec3 : oldShaderPtr->getLocalValueRegistryRef()->fetchRegVec3Names())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesVec3Exist(currentOldVec3))
		{
			cyclerControllerRegistryRef->deleteVec3(currentOldVec3);
		}
	}

	// check mat3
	for (auto& currentOldMat3 : oldShaderPtr->getLocalValueRegistryRef()->fetchRegMat3Names())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesMat3Exist(currentOldMat3))
		{
			cyclerControllerRegistryRef->deleteMat3(currentOldMat3);
		}
	}

	// check mat4
	for (auto& currentOldMat4 : oldShaderPtr->getLocalValueRegistryRef()->fetchRegMat4Names())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesMat4Exist(currentOldMat4))
		{
			cyclerControllerRegistryRef->deleteMat4(currentOldMat4);
		}
	}

	// check float
	for (auto& currentOldFloat : oldShaderPtr->getLocalValueRegistryRef()->fetchRegFloatNames())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesFloatExist(currentOldFloat))
		{
			cyclerControllerRegistryRef->deleteFloat(currentOldFloat);
		}
	}

	// check int
	for (auto& currentOldInt : oldShaderPtr->getLocalValueRegistryRef()->fetchRegIntNames())
	{
		// if it doesn't exist in the new shader, remove it from the cyclerControllerRegistry.
		if (!newShaderPtr->getLocalValueRegistryRef()->doesIntExist(currentOldInt))
		{
			cyclerControllerRegistryRef->deleteInt(currentOldInt);
		}
	}
}

MShaderBase* MShaderSelectionCycler::getTargetShaderRef()
{
	return cycleArray[0];
}

MShaderBase* MShaderSelectionCycler::getPreviousShaderRef()
{
	return cycleArray[1];
}

void MShaderSelectionCycler::iterateLoadedShaders()
{
	if (currentLoadedShaders < 2)
	{
		currentLoadedShaders++;
	}
}

int MShaderSelectionCycler::getNumberOfLoadedShaders()
{
	return currentLoadedShaders;
}