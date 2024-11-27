#include "stdafx.h"
#include "MPointGradient.h"

void MPointGradient::setup()
{
	// Set x ptr
	if (initialValue.x > desiredValue.x)
	{
		xRunPtr = &MPointGradient::runAsHighToLow;
	}
	else if (initialValue.x < desiredValue.x)
	{
		xRunPtr = &MPointGradient::runAsLowToHigh;
	}
	else
	{
		xRunPtr = &MPointGradient::runAsEven;
	}

	// Set y ptr
	if (initialValue.y > desiredValue.y)
	{
		yRunPtr = &MPointGradient::runAsHighToLow;
	}
	else if (initialValue.y < desiredValue.y)
	{
		yRunPtr = &MPointGradient::runAsLowToHigh;
	}
	else
	{
		yRunPtr = &MPointGradient::runAsEven;
	}

	// Set z ptr
	if (initialValue.z > desiredValue.z)
	{
		zRunPtr = &MPointGradient::runAsHighToLow;
	}
	else if (initialValue.z < desiredValue.z)
	{
		zRunPtr = &MPointGradient::runAsLowToHigh;
	}
	else
	{
		zRunPtr = &MPointGradient::runAsEven;
	}
}

void MPointGradient::runAsHighToLow(
	float in_valueA,
	float in_valueB,
	float* in_currentValRef,
	float in_additionalPiValue)
{
	float initialRadians = fullRadians / 4;			// shift the wave left by 1/4 of 2pi, to get the crest.

	// the upward Y shift value, since we are going from high to low, should be in_valueB.
	float upwardShiftValue = in_valueB;

	// the amp wave value is the high - low, divided by 2 -- so in_valueA - in_valueB.
	float ampWaveValue = (in_valueA - in_valueB) / 2;

	// center of wave is equal to the minimum value (in_valueB), + ampWaveValue
	float waveCenter = in_valueB + ampWaveValue;


	*in_currentValRef = (ampWaveValue * (sin(initialRadians + in_additionalPiValue)) + waveCenter);
}

void MPointGradient::runAsLowToHigh(float in_valueA,
	float in_valueB,
	float* in_currentValRef,
	float in_additionalPiValue)
{
	float initialRadians = 3 * (fullRadians / 4);		// the trough will be at 3/4 of 2pi.

	// the upward Y shift value, since we are going from low to high, should be in_valueA.
	float upwardShiftValue = in_valueA;

	// the amp wave value is the high - low, divided by 2 -- so in_valueB - in_valueA.
	float ampWaveValue = (in_valueB - in_valueA) / 2;

	// center of wave is equal to the minimum value (in_valueA), + ampWaveValue
	float waveCenter = in_valueA + ampWaveValue;


	*in_currentValRef = (ampWaveValue * (sin(initialRadians + in_additionalPiValue)) + waveCenter);
}

void MPointGradient::runAsEven(float in_valueA,
	float in_valueB,
	float* in_currentValRef,
	float in_additionalPiValue)
{
	
}

ECBPolyPoint MPointGradient::calculateByTime(float in_piValue)
{
	ECBPolyPoint returnPoint;

	(*this.*xRunPtr)(initialValue.x, desiredValue.x, &returnPoint.x, in_piValue);
	(*this.*yRunPtr)(initialValue.y, desiredValue.y, &returnPoint.y, in_piValue);
	(*this.*zRunPtr)(initialValue.z, desiredValue.z, &returnPoint.z, in_piValue);

	return returnPoint;
}