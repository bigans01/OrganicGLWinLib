#include "stdafx.h"
#include "MGradientController.h"

MGradientController::MGradientController(Message in_gradientInputMessage)
{
	controllerRunMode = MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL;
	selectGradient(in_gradientInputMessage);
}

MGradientController::MGradientController(Message in_gradientInputMessage, float in_piTimeLimit)
{
	controllerRunMode = MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING;
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

Message MGradientController::fetchValueByTime(float in_piTimeValue)
{
	switch (controllerRunMode)
	{
		case MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL:
		{
			std::cout << "Calling CYCLICAL calculateByTime..." << std::endl;
			return selectedMGradientPtr->calculateByTime(in_piTimeValue);
		}

		case MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING:
		{
			finiteTimePassed += in_piTimeValue;
			if (finiteTimePassed >= finiteTimeLimit)
			{
				isGradientFinished = true;
			}
			return selectedMGradientPtr->calculateByTime(in_piTimeValue);
		}
	}
}