#include "stdafx.h"
#include "MShaderSelectionCycler.h"

void MShaderSelectionCycler::setCatalogRef(MShaderCatalog* in_cyclerCatalogRef)
{
	cyclerCatalogRef = in_cyclerCatalogRef;
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
				cycleArray[1] = tempPtr;
				cycleArray[0] = cyclerCatalogRef->getShaderRef(in_shaderName);

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