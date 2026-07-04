#include "stdafx.h"
#include "EmptyNormalFinder.h"

EmptyNormalFinder::EmptyNormalFinder(DoublePoint in_point0, DoublePoint in_point1, DoublePoint in_point2, DoublePoint in_massReferencePoint)
{
	DoublePoint points[3];
	points[0] = in_point0;
	points[1] = in_point1;
	points[2] = in_point2;

	std::set<double> xRanges;
	std::set<double> yRanges;
	std::set<double> zRanges;

	for (int x = 0; x < 3; x++)
	{
		xRanges.insert(points[x].x);
		yRanges.insert(points[x].y);
		zRanges.insert(points[x].z);

		//std::cout << "Double point values were: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
	}

	// the amount to adjust in x/y/z, is the inverse of the minimum found in each dim set.
	// I.e., a triangle point at 320022.0f is going to need to be adjusted by -320000.0f
	double minXAdjustment = ((floor(*xRanges.begin() / 32.0)) * -1.0) * 32.0;
	double minYAdjustment = ((floor(*yRanges.begin() / 32.0)) * -1.0) * 32.0;
	double minZAdjustment = ((floor(*zRanges.begin() / 32.0)) * -1.0) * 32.0;


	DoublePoint translationValue(minXAdjustment, minYAdjustment, minZAdjustment);
	translationValue.roundHundredth();

	//std::cout << "translationValues are:" << translationValue.x << ", " << translationValue.y << ", " << translationValue.z << std::endl;

	// now, translate copies of the points (not the original points), along with the MRP.
	DoublePoint adjustedPoints[3];
	for (int x = 0; x < 3; x++)
	{
		adjustedPoints[x] = points[x] + translationValue;
	}
	DoublePoint adjustedMRP = in_massReferencePoint + translationValue;

	DoublePoint determinedCentroid = IndependentUtils::determineTriangleCentroid(adjustedPoints[0], adjustedPoints[1], adjustedPoints[2]);
	DoublePoint diff = determinedCentroid - adjustedMRP;
	glm::vec3 diffToNormalize(diff.x, diff.y, diff.z);
	diffToNormalize = glm::normalize(diffToNormalize);

	calculatedNormal = diffToNormalize;
}

EmptyNormalFinder::EmptyNormalFinder(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, glm::vec3 in_massReferencePoint)
{
	DoublePoint points[3];
	points[0] = DoublePoint(in_point0);
	points[1] = DoublePoint(in_point1);
	points[2] = DoublePoint(in_point2);

	DoublePoint massReferencePoint = DoublePoint(in_massReferencePoint);

	std::set<double> xRanges;
	std::set<double> yRanges;
	std::set<double> zRanges;

	for (int x = 0; x < 3; x++)
	{
		xRanges.insert(points[x].x);
		yRanges.insert(points[x].y);
		zRanges.insert(points[x].z);

		//std::cout << "Double point values were: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
	}

	// the amount to adjust in x/y/z, is the inverse of the minimum found in each dim set.
	// I.e., a triangle point at 320022.0f is going to need to be adjusted by -320000.0f
	double minXAdjustment = ((floor(*xRanges.begin() / 32.0)) * -1.0) * 32.0;
	double minYAdjustment = ((floor(*yRanges.begin() / 32.0)) * -1.0) * 32.0;
	double minZAdjustment = ((floor(*zRanges.begin() / 32.0)) * -1.0) * 32.0;


	DoublePoint translationValue(minXAdjustment, minYAdjustment, minZAdjustment);
	translationValue.roundHundredth();

	//std::cout << "translationValues are:" << translationValue.x << ", " << translationValue.y << ", " << translationValue.z << std::endl;

	// now, translate copies of the points (not the original points), along with the MRP.
	DoublePoint adjustedPoints[3];
	for (int x = 0; x < 3; x++)
	{
		adjustedPoints[x] = points[x] + translationValue;
	}
	DoublePoint adjustedMRP = massReferencePoint + translationValue;

	DoublePoint determinedCentroid = IndependentUtils::determineTriangleCentroid(adjustedPoints[0], adjustedPoints[1], adjustedPoints[2]);
	DoublePoint diff = determinedCentroid - adjustedMRP;
	glm::vec3 diffToNormalize(diff.x, diff.y, diff.z);
	diffToNormalize = glm::normalize(diffToNormalize);
 
	calculatedNormal = diffToNormalize;
}

glm::vec3 EmptyNormalFinder::getFinderCalculatedNormal()
{
	return calculatedNormal;
}