#include "stdafx.h"
#include "IntersectionLine.h"

void IntersectionLine::addIntersectionResult(IntersectionResult in_result)
{
	//std::cout << "!!!! Add intersection result, point A: " << in_result.intersectedPoint.x << ", " << in_result.intersectedPoint.y << ", " << in_result.intersectedPoint.z << std::endl;

	int currentNoOfPoints = numberOfPoints;
	if (currentNoOfPoints == 0)		// insert into point A
	{
		pointA = in_result.intersectedPoint;
		isPointAOnBorder = in_result.wasIntersectOnBorderLine;
		if (isPointAOnBorder == 1)	// if this intersect was caused by a border line, increment the value appropriately.
		{
			numberOfBorderLines++;
		}
		pointABorder = in_result.borderLineID;
		numberOfPoints++;
	}

	if (currentNoOfPoints == 1)		// insert into point B
	{
		pointB = in_result.intersectedPoint;
		isPointBOnBorder = in_result.wasIntersectOnBorderLine;
		if (isPointBOnBorder == 1) // if this intersect was caused by a border line, increment the value appropriately.
		{
			numberOfBorderLines++;
		}
		pointBBorder = in_result.borderLineID;
		numberOfPoints++;
	}
}

void IntersectionLine::swapBorderToA()
{
	if (isPointBOnBorder == 1)	// only swap borders if B is the one that has a border
	{
		glm::vec3 tempPointA = pointA;				// save values of A, since it's being overwrriten
		int tempPointAOnBorder = isPointAOnBorder;
		int tempPointABorder = pointABorder;

		pointA = pointB;							// overwrite A with B
		isPointAOnBorder = isPointBOnBorder;
		pointABorder = pointBBorder;

		pointB = tempPointA;
		isPointBOnBorder = tempPointAOnBorder;
		pointBBorder = tempPointABorder;
	}
}

void IntersectionLine::swapBorderToB()
{
	if (isPointAOnBorder == 1)
	{
		glm::vec3 tempPointB = pointB;
		int tempPointBOnBorder = isPointBOnBorder;
		int tempPointBBorder = pointBBorder;

		pointB = pointA;
		isPointBOnBorder = isPointAOnBorder;
		pointBBorder = pointABorder;

		pointA = tempPointB;
		isPointAOnBorder = tempPointBOnBorder;
		pointABorder = tempPointBBorder;
	}
}

void IntersectionLine::swapToA()
{
	glm::vec3 tempPointA = pointA;				// save values of A, since it's being overwrriten
	int tempPointAOnBorder = isPointAOnBorder;
	int tempPointABorder = pointABorder;

	pointA = pointB;							// overwrite A with B
	isPointAOnBorder = isPointBOnBorder;
	pointABorder = pointBBorder;

	pointB = tempPointA;
	isPointBOnBorder = tempPointAOnBorder;
	pointBBorder = tempPointABorder;
}

int IntersectionLine::getBorderLineIDFromSingularBorderLineCount()
{
	int result = 0;		// may or may not be 0 in the end, but starts as 0 
	if (isPointAOnBorder == 1)
	{
		result = pointABorder;
	}
	else if (isPointBOnBorder == 1)
	{
		result = pointBBorder;
	}
	return result;
}

glm::vec3 IntersectionLine::getBorderPointFromSingularBorderLineCount()
{
	glm::vec3 borderPoint;
	if (isPointAOnBorder == 1)
	{
		borderPoint = pointA;
	}
	else if (isPointBOnBorder == 1)
	{
		borderPoint = pointB;
	}
	return borderPoint;
}

glm::vec3 IntersectionLine::getNonBorderPointFromSingularBorderLineCount()
{
	glm::vec3 nonBorderPoint;
	if (isPointAOnBorder == 1)
	{
		nonBorderPoint = pointB;
	}
	else if (isPointBOnBorder == 1)
	{
		nonBorderPoint = pointA;
	}
	return nonBorderPoint;
}