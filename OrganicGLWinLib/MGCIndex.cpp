#include "stdafx.h"
#include "MGCIndex.h"

bool MGCIndex::insertCyclicalGradient(std::string in_gradientName, 
									  Message in_cyclicalGradientMessage,
									  float in_singleCycleInPi,
									  float in_piDivisorMs)
{
	bool wasInserted = false;
	auto gradientFinder = gradientControllerMap.find(in_gradientName);
	if (gradientFinder == gradientControllerMap.end())	// it wasn't found, insert.
	{
		MGradientController newController(in_cyclicalGradientMessage, in_singleCycleInPi, in_piDivisorMs);
		gradientControllerMap[in_gradientName] = newController;
		wasInserted = true;
	}
	return wasInserted;
}

bool MGCIndex::insertFiniteGradient(std::string in_gradientName, 
									Message in_finiteGradientMessage, 
									float in_singleCycleInPi,
									float in_piDivisorMs,
									float in_finiteDuration)
{
	bool wasInserted = false;
	auto gradientFinder = gradientControllerMap.find(in_gradientName);
	if (gradientFinder == gradientControllerMap.end())	// it wasn't found, insert.
	{
		MGradientController newController(in_finiteGradientMessage, in_singleCycleInPi, in_piDivisorMs, in_finiteDuration);
		gradientControllerMap[in_gradientName] = newController;
		wasInserted = true;
	}
	return wasInserted;
}

bool MGCIndex::doesGradientExist(std::string in_gradientName)
{
	bool gradientExists = false;
	auto gradientFinder = gradientControllerMap.find(in_gradientName);
	if (gradientFinder != gradientControllerMap.end())	// it wasn't found, insert.
	{
		gradientExists = true;
	}
	return gradientExists;
}

Message MGCIndex::fetchGradientValueFromController(std::string in_gradientName, float in_piTimeValue)
{
	return gradientControllerMap[in_gradientName].fetchValueByTime(in_piTimeValue);
}

void MGCIndex::deleteExpiredFiniteGradients()
{
	std::vector<std::string> deletableGradients;
	for (auto& currentGradient : gradientControllerMap)
	{
		// delete only if it's finite AND finished.
		if 
		(
			(currentGradient.second.getControllerRunMode() == MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING)
			&&
			(currentGradient.second.checkIfFinished())
		)
		{
			deletableGradients.push_back(currentGradient.first);
		}
	}

	for (auto& currentDeletable : deletableGradients)
	{
		gradientControllerMap.erase(currentDeletable);
	}
}