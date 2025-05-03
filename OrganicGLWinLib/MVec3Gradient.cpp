#include "stdafx.h"
#include "MVec3Gradient.h"

void MVec3Gradient::initializeGradient(Message in_gradientInfo)
{
	// The Message should be of the type MGRADIENT_VEC3_INPUT, and contain two ECBPolyPoints to read from.
	in_gradientInfo.open();
	initialValue = in_gradientInfo.readPoint();
	desiredValue = in_gradientInfo.readPoint();

	// Set x ptr
	if (initialValue.x > desiredValue.x)
	{
		xRunPtr = &MVec3Gradient::runAsHighToLow;
	}
	else if (initialValue.x < desiredValue.x)
	{
		xRunPtr = &MVec3Gradient::runAsLowToHigh;
	}
	else
	{
		xRunPtr = &MVec3Gradient::runAsEven;
	}

	// Set y ptr
	if (initialValue.y > desiredValue.y)
	{
		yRunPtr = &MVec3Gradient::runAsHighToLow;
	}
	else if (initialValue.y < desiredValue.y)
	{
		yRunPtr = &MVec3Gradient::runAsLowToHigh;
	}
	else
	{
		yRunPtr = &MVec3Gradient::runAsEven;
	}

	// Set z ptr
	if (initialValue.z > desiredValue.z)
	{
		zRunPtr = &MVec3Gradient::runAsHighToLow;
	}
	else if (initialValue.z < desiredValue.z)
	{
		zRunPtr = &MVec3Gradient::runAsLowToHigh;
	}
	else
	{
		zRunPtr = &MVec3Gradient::runAsEven;
	}
}

void MVec3Gradient::runAsHighToLow(
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

void MVec3Gradient::runAsLowToHigh(float in_valueA,
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

void MVec3Gradient::runAsEven(float in_valueA,
	float in_valueB,
	float* in_currentValRef,
	float in_additionalPiValue)
{

}

Message MVec3Gradient::calculateByTime(float in_piValue)
{
	ECBPolyPoint returnPoint;

	(*this.*xRunPtr)(initialValue.x, desiredValue.x, &returnPoint.x, in_piValue);
	(*this.*yRunPtr)(initialValue.y, desiredValue.y, &returnPoint.y, in_piValue);
	(*this.*zRunPtr)(initialValue.z, desiredValue.z, &returnPoint.z, in_piValue);

	//std::cout << "!! Finished with MVec3Gradient::calculateByTime, returning Message..." << std::endl;

	return Message(MessageType::MGRADIENT_VEC3_OUTPUT, returnPoint);
}