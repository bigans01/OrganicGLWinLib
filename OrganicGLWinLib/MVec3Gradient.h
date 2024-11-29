#pragma once

#ifndef MVEC3GRADIENT_H
#define MVEC3GRADIENT_H

#include "MGradientBase.h"

/*

Description: This class uses sin waves to create a point, the value of which is based on a cycle that is determined
from the two input points from a MGRADIENT_VEC3_INPUT Message.

Passing a pi based float value in to the calculateByTime function should return a Message of the type MGRADIENT_VEC3_OUTPUT,
which contains a single ECBPolyPoint (that can represent glm::vec3); this ECBPolyPoint will be the value of the point in the cycle,
at the time point given as input by the call to calculateByTime.

*/

class MVec3Gradient : public MGradientBase
{
	public:
		// virtual functions, from MGradientBase
		void initializeGradient(Message in_gradientInfo);	// should be MessageType::MGRADIENT_VEC3_INPUT
		Message calculateByTime(float in_piValue);			// return value should be MessageType::MGRADIENT_VEC3_OUTPUT
	private:
		ECBPolyPoint initialValue;		// the initial point of the wave
		ECBPolyPoint desiredValue;		// the desired point to get to when pi time has passed.

		// function pointers, that are used after the appropriate "run" functions below are selected.
		void(MVec3Gradient::*xRunPtr)(float, float, float*, float) = nullptr;
		void(MVec3Gradient::*yRunPtr)(float, float, float*, float) = nullptr;
		void(MVec3Gradient::*zRunPtr)(float, float, float*, float) = nullptr;


		void runAsHighToLow(float in_valueA,	// called when going from a higher value to a lower one
			float in_valueB,
			float* in_currentValRef,
			float in_additionalPiValue);

		void runAsLowToHigh(float in_valueA,	// called when going from a lower value to a higher one
			float in_valueB,
			float* in_currentValRef,
			float in_additionalPiValue);

		void runAsEven(float in_valueA,			// called when the values are the same
			float in_valueB,
			float* in_currentValRef,
			float in_additionalPiValue);
};


#endif
