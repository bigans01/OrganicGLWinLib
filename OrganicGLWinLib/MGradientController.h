#pragma once

#ifndef MGRADIENTCONTROLLER_H
#define MGRADIENTCONTROLLER_H

#include "MGradientBase.h"
#include "MVec3Gradient.h"
#include "MGradientRunMode.h"

/*

Description: a management class that is built an around a std::shared_ptr<MGradientBase>. The controller manages this pointer,
but uses additional metadata that can affect how the gradient should behave (ie., will it be finite or cyclical.

Whether or not it is a finite or cyclical mode depends on how the MGradientController is instantiated from one of the two non-default
constructors.

*/

class MGradientController
{
	public:
		MGradientController() {};	// copy constructor, so this class can go into a map; don't initialize with this call, 
									// use MGradientController(MGradientRunMode in_gradientRunMode, Message in_gradientInputMessage).


		// used when wanting a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_CYCLICAL.
		MGradientController(Message in_gradientInputMessage, 
							float in_singleCycleInPi, 
							float in_piDivisorMs);	

		// used when wanting a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING.
		MGradientController(Message in_gradientInputMessage, 
							float in_singleCycleInPi, 
							float in_piDivisorMs, 
							float in_piTimeLimit);	

		Message fetchValueByTime(float in_timeInMs);
		MGradientRunMode getControllerRunMode();
		bool checkIfFinished();	// only returns true when the controllerRunMode is working in MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING
	private:
		void selectGradient(Message in_gradientInputMessage);

		MGradientRunMode controllerRunMode = MGradientRunMode::UNSET;	// MUST be set by a valid value in the call to MGradientController non-default constructor. (see MGradientRunMode.h)
		bool isGradientFinished = false;	// could be set to true, only when the MGradientRunMode is set to RUN_GRADIENT_AS_FINITE.
		float finiteTimePassed = 0.0f;		// the amount of time that has passed, which is incremented in every call to fetchValueByTime, when running in a controllerRunMode of MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING.
		float finiteTimeLimit = 0.0f;		// will be equal to in_piTimeDuration from the call 2nd non-default constructor of this class
		float cyclicalPiValue = 0.0f;
		std::shared_ptr<MGradientBase> selectedMGradientPtr = nullptr;

		float singleCycleInPi = 0.0f;	// a float represent the number of radians that equates to a single cycle (i.e 2pi in radians)
		float piDivisorMs = 0.0f;		// a float used to divide singleCycleInPi; the resulting calculation gets us piPerMs.
		float piPerMs = 0.0f;			// used to multiply with in_timeInMs when fetchValueByTime is called
		float controllerFullRadians = 6.28319f;	// the equivalent of 2pi (roughly)

		float determinePiValueForGradientCalculation(float in_timeElapsedInMs);	// when working with MGradientRunMode::RUN_GRADIENT_AS_FINITE_INCREMENTING,
																				// and passing in the value of finiteTimePassed, this will give a value at a certain 
																				// point in finite time length.

		float determinePiValueForCyclicalCalculation(float in_timeElapsedInMs);	// uses the input parameter to determine how much to add to cyclicalPiValue;
																				// if the value of cyclicalPiValue is greater than 2pi, use fmod to get the remainder, to
																				// effectively recalibrate the cycle.
};

#endif
