#include "stdafx.h"
#include "MShaderController.h"

bool MShaderController::switchOShader(std::string in_shaderToSwitchTo)
{
	return true;
}

bool MShaderController::addOShaderToCatalog(std::string in_shaderToSwitchTo)
{
	return true;
}

void MShaderController::setupSharedComponents()
{

}

void MShaderController::instantiateAndSetupShader(std::string in_shaderToInstantiate)
{

}

bool MShaderController::BindingMap::doesBindingExist(std::string in_bindingToFind)
{
	bool wasFound = false;
	auto bindingFinder = bindings.find(in_bindingToFind);
	if (bindingFinder != bindings.end())
	{
		wasFound = true;
	}
	return wasFound;
}

void MShaderController::BindingMap::insertBinding(std::string in_bindingName, int in_bindingValue)
{
	bindings[in_bindingName] = in_bindingValue;
}

void MShaderController::processMessage(Message in_messageToRead)
{

};