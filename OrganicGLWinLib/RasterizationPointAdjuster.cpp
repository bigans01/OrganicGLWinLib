#include "stdafx.h"
#include "RasterizationPointAdjuster.h"

void RasterizationPointAdjuster::runFitScans()
{
	runFitScanX();
	runFitScanY();
}
	
void RasterizationPointAdjuster::runFitScanX()
{
	float minBorder = currentMaxNegPosBorder * -1;
	float maxBorder = currentMaxNegPosBorder;

	float currentMinValue = minBorder;
	float currentMaxValue = maxBorder;

	bool minBorderActivatedFlag = false;
	bool maxBorderActivatedFlag = false;

	auto pointsRefBegin = pointsRef->getPointsVectorBegin();
	auto pointsRefEnd = pointsRef->getPointsVectorEnd();
	for (; pointsRefBegin != pointsRefEnd; pointsRefBegin++)
	{
		// check for min on x
		if ((*pointsRefBegin)->x < currentMinValue)
		{
			minBorderActivatedFlag = true;
			currentMinValue = (*pointsRefBegin)->x;
		}

		// check for max on x
		if ((*pointsRefBegin)->x > currentMaxValue)
		{
			maxBorderActivatedFlag = true;
			currentMaxValue = (*pointsRefBegin)->x;
		}
	}

	// check if we need to scale, and if we do, find the downsizingScaleValue
	float downsizingScaleValue = 0.0f;
	bool isScalingNeeded = false;

	// CASE 1: both minBorderActivatedFlag and maxBorderActivatedFlag are true

	// sample logic:
	// if the min border is -2, but the currentMinValue is -3, you would need to downscale on X by -2/-3, which is 66 percent (~0.66666f). 
	// We would then multiply the X values of all the points by this value (0.66666f), to guarantee that they are within the border.

	if 
	(
		(minBorderActivatedFlag == true)
		&&
		(maxBorderActivatedFlag == true)
	)
	{
		isScalingNeeded = true;

		// need to find which value is the furthest from the center point at 0,0, 
		float currentMinAbsolute = abs(currentMinValue);
		float currentMax = currentMaxValue;
		float highestXOutlier = std::max(currentMinAbsolute, currentMax);

		if (highestXOutlier == currentMinAbsolute)
		{
			downsizingScaleValue = abs(minBorder / currentMinValue);	// downsizingScaleValue must always be absolute, can't be - 
		}
		else if (highestXOutlier == currentMax)
		{
			downsizingScaleValue = maxBorder / currentMax;
		}

	}

	// CASE 2: minBorderaActivedFlag is the only one flagged
	else if (minBorderActivatedFlag == true)
	{
		isScalingNeeded = true;
		downsizingScaleValue = abs(minBorder / currentMinValue);	 // downsizingScaleValue must always be absolute, can't be - 
	}

	// CASE 3: maxBorderaActivedFlag is the only one flagged
	else if (maxBorderActivatedFlag == true)
	{
		isScalingNeeded = true;
		downsizingScaleValue = maxBorder / currentMaxValue;
	}

	// if the isScalingNeeded flag is true, it means we need to multiply all X values in each point by the downsizingScaleValue.
	if (isScalingNeeded == true)
	{
		// first pass: multiply all X values by downsizingScaleValue
		auto pointsToScaleBegin = pointsRef->getPointsVectorBegin();
		auto pointsToScaleEnd = pointsRef->getPointsVectorEnd();
		for (; pointsToScaleBegin != pointsToScaleEnd; pointsToScaleBegin++)
		{
			(*pointsToScaleBegin)->x *= downsizingScaleValue;
		}

		// second pass: "sand" any X values so they fit within the boundaries of the rasterization

	}

}

void RasterizationPointAdjuster::runFitScanY()
{

}