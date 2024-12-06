#include "stdafx.h"
#include "MGradientController.h"

MGradientController::MGradientController(Message in_gradientInputMessage, float in_singleCycleInPi, float in_piDivisorMs)
{
	controllerRunMode = MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL;
	singleCycleInPi = in_singleCycleInPi;
	piDivisorMs = in_piDivisorMs;
	piPerMs = in_singleCycleInPi / in_piDivisorMs;
	selectGradient(in_gradientInputMessage);
}

MGradientController::MGradientController(Message in_gradientInputMessage, float in_singleCycleInPi, float in_piDivisorMs, float in_piTimeLimit)
{
	controllerRunMode = MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING;
	singleCycleInPi = in_singleCycleInPi;
	piDivisorMs = in_piDivisorMs;
	piPerMs = in_singleCycleInPi / in_piDivisorMs;
	finiteTimeLimit = in_piTimeLimit;
	selectGradient(in_gradientInputMessage);
}

void MGradientController::selectGradient(Message in_gradientInputMessage)
{
	std::cout << "Selecting gradient..." << std::endl;
	switch (in_gradientInputMessage.messageType)
	{
		case MessageType::MGRADIENT_VEC3_INPUT:
		{
			std::cout << "Select MVec3Gradient. " << std::endl;
			selectedMGradientPtr = std::shared_ptr<MGradientBase>(new MVec3Gradient());
			selectedMGradientPtr->initializeGradient(in_gradientInputMessage);
			break;
		};
	}
}

Message MGradientController::fetchValueByTime(float in_timeInMs)
{
	switch (controllerRunMode)
	{
		case MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL:
		{
			std::cout << "Calling CYCLICAL calculateByTime..." << std::endl;
			return selectedMGradientPtr->calculateByTime(determinePiValueForCyclicalCalculation(in_timeInMs));
		}

		case MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING:
		{
			std::cout << "Calling FINITE calculateByTime..." << std::endl;
			finiteTimePassed += in_timeInMs;
			float valueForCalculation = determinePiValueForGradientCalculation(finiteTimePassed);
			if (finiteTimePassed >= finiteTimeLimit)
			{
				isGradientFinished = true;
			}
			return selectedMGradientPtr->calculateByTime(valueForCalculation);
		}
	}
}

MGradientRunMode MGradientController::getControllerRunMode()
{
	return controllerRunMode;
}

bool MGradientController::checkIfFinished()
{
	return isGradientFinished;
}

float MGradientController::determinePiValueForGradientCalculation(float in_timeElapsedInMs)
{
	return (piPerMs * in_timeElapsedInMs);
}

float MGradientController::determinePiValueForCyclicalCalculation(float in_timeElapsedInMs)
{
	// Below: if we go beyond 2pi, use the remainder of what we added to determine the
	// offset we will use to go into the next cycle.
	//
	// Otherwise, just use whatever the value of cyclicalPiValue is.
	cyclicalPiValue += (piPerMs * in_timeElapsedInMs);

	if (cyclicalPiValue >= controllerFullRadians)
	{
		cyclicalPiValue = fmod(cyclicalPiValue, controllerFullRadians);
	}

	return cyclicalPiValue;
}