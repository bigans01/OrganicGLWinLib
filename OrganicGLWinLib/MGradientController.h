#pragma once

#ifndef MGRADIENTCONTROLLER_H
#define MGRADIENTCONTROLLER_H

#include "MGradientBase.h"
#include "MVec3Gradient.h"
#include "MGradientRunMode.h"

class MGradientController
{
	public:
		MGradientController() {};	// copy constructor, so this class can go into a map; don't initialize with this call, 
									// use MGradientController(MGradientRunMode in_gradientRunMode, Message in_gradientInputMessage).
		MGradientController(Message in_gradientInputMessage);	// used when wanting a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL.
		MGradientController(Message in_gradientInputMessage, float in_piTimeLimit);	// used when wanting a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING;
																						// the second float value is the amount of time in pi, to increment by. Each increment will add to the 

		Message fetchValueByTime(float in_piTimeValue);	// the input to this function should be a value of pi, where the pi value is based on time elapsed.
		
	private:
		void selectGradient(Message in_gradientInputMessage);

		MGradientRunMode controllerRunMode = MGradientRunMode::UNSET;	// MUST be set by a valid value in the call to MGradientController non-default constructor. (see MGradientRunMode.h)
		bool isGradientFinished = false;	// could be set to true, only when the MGradientRunMode is set to RUN_GRADIENT_AS_FINITE.
		float finiteTimePassed = 0.0f;		// the amount of time that has passed, which is incremented in every call to fetchValueByTime, when running in a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING.
		float finiteTimeLimit = 0.0f;		// will be equal to in_piTimeDuration from the call 2nd non-default constructor of this class
		std::shared_ptr<MGradientBase> selectedMGradientPtr = nullptr;
};

#endif
