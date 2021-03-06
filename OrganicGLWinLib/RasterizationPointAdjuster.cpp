#include "stdafx.h"
#include "RasterizationPointAdjuster.h"

void RasterizationPointAdjuster::runFitScans()
{
	runFitScanX();
	runFitScanY();
}

void RasterizationPointAdjuster::applyDownsizeFactor()
{
	//std::cout << "applying downsize factor, having a value of: " << downsizeFactor << std::endl;

	auto pointsToScaleBegin = pointsRef->getPointsVectorBegin();
	auto pointsToScaleEnd = pointsRef->getPointsVectorEnd();
	for (; pointsToScaleBegin != pointsToScaleEnd; pointsToScaleBegin++)
	{
		(*pointsToScaleBegin)->x *= downsizeFactor;
		(*pointsToScaleBegin)->y *= downsizeFactor;
	}
}

void RasterizationPointAdjuster::translatePointsBackToRasterizationBox()
{
	auto pointsToScaleBegin = pointsRef->getPointsVectorBegin();
	auto pointsToScaleEnd = pointsRef->getPointsVectorEnd();
	for (; pointsToScaleBegin != pointsToScaleEnd; pointsToScaleBegin++)
	{
		//(*pointsToScaleBegin)->x += rasterizationPlaneCenterPoint.x;
		//(*pointsToScaleBegin)->y += rasterizationPlaneCenterPoint.y;
		**pointsToScaleBegin += rasterizationPlaneCenterPoint;
	}
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
	isScalingNeeded = false;

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
		//std::cout << "!!! Scaling on X needed..." << std::endl;

		// first pass: multiply all X values by downsizingScaleValue
		auto pointsToScaleBegin = pointsRef->getPointsVectorBegin();
		auto pointsToScaleEnd = pointsRef->getPointsVectorEnd();
		for (; pointsToScaleBegin != pointsToScaleEnd; pointsToScaleBegin++)
		{
			std::cout << "!! X value, prior to downsizing: " << (*pointsToScaleBegin)->x;

			(*pointsToScaleBegin)->x *= downsizingScaleValue;

			std::cout << "!! X value, after downsizing: " << (*pointsToScaleBegin)->x;
		}

		// second pass: "sand" any X values so they fit within the boundaries of the rasterization
		auto secondPassScaleBegin = pointsRef->getPointsVectorBegin();
		auto secondPassScaleEnd = pointsRef->getPointsVectorEnd();
		for (; secondPassScaleBegin != secondPassScaleEnd; secondPassScaleBegin++)
		{
			if ((*secondPassScaleBegin)->x > maxBorder)
			{
				std::cout << "Adjustment: x is greater than max border. " << std::endl;
				(*secondPassScaleBegin)->x = maxBorder;
			}
			else if ((*secondPassScaleBegin)->x < minBorder)
			{

				std::cout << "Adjustment: x is less than min border. " << std::endl;
				(*secondPassScaleBegin)->x = minBorder;
			}
		}
	}

}

void RasterizationPointAdjuster::runFitScanY()
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
		// check for min on y
		if ((*pointsRefBegin)->y < currentMinValue)
		{
			minBorderActivatedFlag = true;
			currentMinValue = (*pointsRefBegin)->y;
		}

		// check for max on y
		if ((*pointsRefBegin)->y > currentMaxValue)
		{
			maxBorderActivatedFlag = true;
			currentMaxValue = (*pointsRefBegin)->y;
		}
	}

	// check if we need to scale, and if we do, find the downsizingScaleValue
	float downsizingScaleValue = 0.0f;
	isScalingNeeded = false;

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
		float highestYOutlier = std::max(currentMinAbsolute, currentMax);

		if (highestYOutlier == currentMinAbsolute)
		{
			downsizingScaleValue = abs(minBorder / currentMinValue);	// downsizingScaleValue must always be absolute, can't be - 
		}
		else if (highestYOutlier == currentMax)
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
		//std::cout << "!!! Scaling on Y needed..." << std::endl;

		// first pass: multiply all X values by downsizingScaleValue
		auto pointsToScaleBegin = pointsRef->getPointsVectorBegin();
		auto pointsToScaleEnd = pointsRef->getPointsVectorEnd();
		for (; pointsToScaleBegin != pointsToScaleEnd; pointsToScaleBegin++)
		{
			(*pointsToScaleBegin)->y *= downsizingScaleValue;
		}

		// second pass: "sand" any Y values so they fit within the boundaries of the rasterization
		auto secondPassScaleBegin = pointsRef->getPointsVectorBegin();
		auto secondPassScaleEnd = pointsRef->getPointsVectorEnd();
		for (; secondPassScaleBegin != secondPassScaleEnd; secondPassScaleBegin++)
		{
			if ((*secondPassScaleBegin)->y > maxBorder)
			{
				std::cout << "Adjustment: y is greater than max border. " << std::endl;
				(*secondPassScaleBegin)->y = maxBorder;
			}
			else if ((*secondPassScaleBegin)->y < minBorder)
			{
				std::cout << "Adjustment: y is less than min border. " << std::endl;
				(*secondPassScaleBegin)->y = minBorder;
			}
		}
	}
}