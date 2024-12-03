#pragma once

#ifndef MGCINDEX_H
#define MGCINDEX_H

#include "MGradientController.h"

/*

Description: This class stores and manages various MGradientController objects that 
can be used by the MShaderController for transiting variables from one value to another.

*/

class MGCIndex
{
	public:
		MGCIndex() {};
		bool insertCyclicalGradient(std::string in_gradientName, 
									Message in_cyclicalGradientMessage,
									float in_singleCycleInPi,
									float in_piDivisorMs);	// insert a cyclical gradient; the type of gradient 
																										// is based off the Message contents

		bool insertFiniteGradient(std::string in_gradientName, 
								  Message in_finiteGradientMessage, 
									float in_singleCycleInPi,
									float in_piDivisorMs,
									float in_finiteDuration);	// insert a finite gradient; the type of
																															// gradient is determined by the Message contents,
																															// and the float value represents how long the gradient should last in pi time.
		bool doesGradientExist(std::string in_gradientName);	// returns true if a gradient already exists.
		Message fetchGradientValueFromController(std::string in_gradientName, float in_piTimeValue);	// get the current value belonging to a gradient, by pi time duration.
		void deleteExpiredFiniteGradients();	// check if any gradients have expired, and if so, delete them.
	private:
		std::unordered_map<std::string, MGradientController> gradientControllerMap;
};

#endif
