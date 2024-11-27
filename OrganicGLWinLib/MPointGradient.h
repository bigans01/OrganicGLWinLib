#pragma once

#ifndef MPOINTGRADIENT_H
#define MPOINTGRADIENT_H

/* Description: 

The MPointGradient class serves as an object that takes in two points; the x/y/z values of each point 
are compared to each other. 

If the value of an initial dimension such as x is higher than the desired dimension of x, 
A sin wave that is shifted to the left by 1/4 of 2pi will be created. 

The opposite holds true when an initial dimension from the initialVlue is lower than its 
counterpart iimension in desiredValue.

In order to get what the value is at any point in time, simply call the calculateByTime function, 
where the float value is some multiple of pi. Calling pi as the argument to calculateByTime will result in
a value that is equivalent to the desiredValue.

*/

class MPointGradient
{
	public:
		MPointGradient(ECBPolyPoint in_initialValue,
						ECBPolyPoint in_desiredValue) :
			initialValue(in_initialValue),
			desiredValue(in_desiredValue)
		{
			setup();
		};

		ECBPolyPoint calculateByTime(float in_piValue);		// returns the value of the point after the 
															// given wave calculation, with the value of time passed measured in pi.
	private:
		ECBPolyPoint initialValue;		// the initial point of the wave
		ECBPolyPoint desiredValue;		// the desired point to get to when pi time has passed.

		const float fullRadians = 6.28319f;	// the equivalent of 2pi

		void setup();	// sets the value of the x/y/zRunPtr values below

		// function pointers, that are used after the appropriate "run" functions below are selected.
		void(MPointGradient::*xRunPtr)(float, float, float*, float) = nullptr;
		void(MPointGradient::*yRunPtr)(float, float, float*, float) = nullptr;
		void(MPointGradient::*zRunPtr)(float, float, float*, float) = nullptr;


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
