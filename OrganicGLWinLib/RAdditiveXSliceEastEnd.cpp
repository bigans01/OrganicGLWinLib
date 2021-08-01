#include "stdafx.h"
#include "RAdditiveXSliceEastEnd.h"

void RAdditiveXSliceEastEnd::buildInitialPointSets()
{
	// order of points for X slice is: 
	// first corner	 = 4, 0
	// second corner = 0, 0
	// third corner  = 0, 4
	// fourth corner = 4, 4

	// Value of X for the setA is always equal to sliceIndex * sliceThickness
	float currentSliceSetX = sliceIndex * sliceThickness;
	glm::vec3 setACorner0(currentSliceSetX, 4 * sliceThickness, 0 * sliceThickness);
	glm::vec3 setACorner1(currentSliceSetX, 0 * sliceThickness, 0 * sliceThickness);
	glm::vec3 setACorner2(currentSliceSetX, 0 * sliceThickness, 4 * sliceThickness);
	glm::vec3 setACorner3(currentSliceSetX, 4 * sliceThickness, 4 * sliceThickness);

	std::cout << "---- Set A corners for this slice: " << std::endl;
	std::cout << "corner 0: " << setACorner0.x << ", " << setACorner0.y << ", " << setACorner0.z << std::endl;
	std::cout << "corner 1: " << setACorner1.x << ", " << setACorner1.y << ", " << setACorner1.z << std::endl;
	std::cout << "corner 2: " << setACorner2.x << ", " << setACorner2.y << ", " << setACorner2.z << std::endl;
	std::cout << "corner 3: " << setACorner3.x << ", " << setACorner3.y << ", " << setACorner3.z << std::endl;


	glm::vec3 setBCorner0(currentSliceSetX + sliceThickness, 4 * sliceThickness, 0 * sliceThickness);
	glm::vec3 setBCorner1(currentSliceSetX + sliceThickness, 0 * sliceThickness, 0 * sliceThickness);
	glm::vec3 setBCorner2(currentSliceSetX + sliceThickness, 0 * sliceThickness, 4 * sliceThickness);
	glm::vec3 setBCorner3(currentSliceSetX + sliceThickness, 4 * sliceThickness, 4 * sliceThickness);

	std::cout << "---- Set B corners for this slice: " << std::endl;
	std::cout << "corner 0: " << setBCorner0.x << ", " << setBCorner0.y << ", " << setBCorner0.z << std::endl;
	std::cout << "corner 1: " << setBCorner1.x << ", " << setBCorner1.y << ", " << setBCorner1.z << std::endl;
	std::cout << "corner 2: " << setBCorner2.x << ", " << setBCorner2.y << ", " << setBCorner2.z << std::endl;
	std::cout << "corner 3: " << setBCorner3.x << ", " << setBCorner3.y << ", " << setBCorner3.z << std::endl;
	generateSetBRCollisionPoints(numberOfPointsBetweenCorners, setBCorner0, setBCorner1, setBCorner2, setBCorner3);
}

void RAdditiveXSliceEastEnd::buildPointSets()
{
	// find the min/max values for y and z.
	std::cout << "Building point sets for X_SLICE. " << std::endl;

	auto keyedMorphablesBegin = sliceMorphableKeys.begin();
	auto keyedMorphablesEnd = sliceMorphableKeys.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		yLimitMin = std::min(yLimitMin, keyedMorphablesBegin->y);
		yLimitMax = std::max(yLimitMax, keyedMorphablesBegin->y);

		zLimitMin = std::min(zLimitMin, keyedMorphablesBegin->z);
		zLimitMax = std::max(zLimitMax, keyedMorphablesBegin->z);
	}

	std::cout << "Y limits: min -> " << yLimitMin << " | max -> " << yLimitMax << std::endl;
	std::cout << "Z limits: min -> " << zLimitMin << " | max -> " << zLimitMax << std::endl;

	// order of points for X slice is: 
	// first corner	 = yLimitMax, zLimitMin
	// second corner = yLimitMin, zLimitMin
	// third corner  = yLimitMin, zLimitMax
	// fourth corner = yLimitMax, zLimitMax

	// Value of X for the setA is always equal to sliceIndex * sliceThickness
	float currentSliceSetX = sliceIndex * sliceThickness;
	glm::vec3 setACorner0(currentSliceSetX, yLimitMax * sliceThickness, zLimitMin * sliceThickness);
	glm::vec3 setACorner1(currentSliceSetX, yLimitMin * sliceThickness, zLimitMin * sliceThickness);
	glm::vec3 setACorner2(currentSliceSetX, yLimitMin * sliceThickness, zLimitMax * sliceThickness);
	glm::vec3 setACorner3(currentSliceSetX, yLimitMax * sliceThickness, zLimitMax * sliceThickness);

	std::cout << "Set A corners for this slice: " << std::endl;
	std::cout << "corner 0: " << setACorner0.x << ", " << setACorner0.y << ", " << setACorner0.z << std::endl;
	std::cout << "corner 1: " << setACorner1.x << ", " << setACorner1.y << ", " << setACorner1.z << std::endl;
	std::cout << "corner 2: " << setACorner2.x << ", " << setACorner2.y << ", " << setACorner2.z << std::endl;
	std::cout << "corner 3: " << setACorner3.x << ", " << setACorner3.y << ", " << setACorner3.z << std::endl;

	glm::vec3 setBCorner0(currentSliceSetX + sliceThickness, yLimitMax * sliceThickness, zLimitMin * sliceThickness);
	glm::vec3 setBCorner1(currentSliceSetX + sliceThickness, yLimitMin * sliceThickness, zLimitMin * sliceThickness);
	glm::vec3 setBCorner2(currentSliceSetX + sliceThickness, yLimitMin * sliceThickness, zLimitMax * sliceThickness);
	glm::vec3 setBCorner3(currentSliceSetX + sliceThickness, yLimitMax * sliceThickness, zLimitMax * sliceThickness);

	std::cout << "---- Set B corners for this slice: " << std::endl;
	std::cout << "corner 0: " << setBCorner0.x << ", " << setBCorner0.y << ", " << setBCorner0.z << std::endl;
	std::cout << "corner 1: " << setBCorner1.x << ", " << setBCorner1.y << ", " << setBCorner1.z << std::endl;
	std::cout << "corner 2: " << setBCorner2.x << ", " << setBCorner2.y << ", " << setBCorner2.z << std::endl;
	std::cout << "corner 3: " << setBCorner3.x << ", " << setBCorner3.y << ", " << setBCorner3.z << std::endl;
	generateSetBRCollisionPoints(numberOfPointsBetweenCorners, setBCorner0, setBCorner1, setBCorner2, setBCorner3);
}

void RAdditiveXSliceEastEnd::generateSetBRCollisionPoints(int in_numberOfPointsBetweenCorners,
	glm::vec3 in_corner0,
	glm::vec3 in_corner1,
	glm::vec3 in_corner2,
	glm::vec3 in_corner3)
{
	// X value is the same for all points in the set, for this slice
	float pointX = (sliceIndex * sliceThickness) + sliceThickness;
	int currentPointIndex = 0;


	// corner 0 logic
	ECBPolyPoint corner0Converted(in_corner0.x, in_corner0.y, in_corner0.z);
	ECBPPOrientationResults corner0Orientation = IndependentUtils::getDynamicPointOrientation(corner0Converted, sliceDynBorderRef);
	RCollisionPoint* corner0PointRef = pointArrayRef->attemptPointInsert(in_corner0, corner0Orientation);
	typicalPointSetB[currentPointIndex++] = corner0PointRef;
	std::cout << "!! Inserted corner point 0: " << in_corner0.x << ", " << in_corner0.y << ", " << in_corner0.z << std::endl;

	// any points between corner 0 and corner 1 (difference in points between these corners is -y)
	float corner0BasedCurrentY = in_corner0.y;
	float corner0PointZ = in_corner0.z;
	//float corner0YDecrementValue = float(yLimitMax - yLimitMin) / float(in_numberOfPointsBetweenCorners + 1);
	float corner0YDecrementValue = dimLimit / float(in_numberOfPointsBetweenCorners + 1);
	for (int x = 0; x < in_numberOfPointsBetweenCorners; x++)
	{
		corner0BasedCurrentY -= corner0YDecrementValue;
		glm::vec3 glmPoint(pointX, corner0BasedCurrentY, corner0PointZ);
		ECBPolyPoint newPoint(pointX, corner0BasedCurrentY, corner0PointZ);
		ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(newPoint, sliceDynBorderRef);
		RCollisionPoint* currentPointRef = pointArrayRef->attemptPointInsert(glmPoint, currentPointOrientation);
		typicalPointSetB[currentPointIndex++] = currentPointRef;
		std::cout << "!! Inserted new point: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
	}

	// corner 1 logic
	ECBPolyPoint corner1Converted(in_corner1.x, in_corner1.y, in_corner1.z);
	ECBPPOrientationResults corner1Orientation = IndependentUtils::getDynamicPointOrientation(corner1Converted, sliceDynBorderRef);
	RCollisionPoint* corner1PointRef = pointArrayRef->attemptPointInsert(in_corner1, corner1Orientation);
	typicalPointSetB[currentPointIndex++] = corner1PointRef;
	std::cout << "!! Inserted corner point 1: " << in_corner1.x << ", " << in_corner1.y << ", " << in_corner1.z << std::endl;

	// any points between corner 1 and corner 2 (difference in points between these corners is +z)
	float corner1BasedCurrentZ = in_corner1.z;
	float corner1PointY = in_corner1.y;
	//float corner1ZIncrementValue = float(zLimitMax - zLimitMin) / float(in_numberOfPointsBetweenCorners + 1);
	float corner1ZIncrementValue = dimLimit / float(in_numberOfPointsBetweenCorners + 1);
	for (int x = 0; x < in_numberOfPointsBetweenCorners; x++)
	{
		corner1BasedCurrentZ += corner1ZIncrementValue;
		glm::vec3 glmPoint(pointX, corner1PointY, corner1BasedCurrentZ);
		ECBPolyPoint newPoint(pointX, corner1PointY, corner1BasedCurrentZ);
		ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(newPoint, sliceDynBorderRef);
		RCollisionPoint* currentPointRef = pointArrayRef->attemptPointInsert(glmPoint, currentPointOrientation);
		typicalPointSetB[currentPointIndex++] = currentPointRef;
		std::cout << "!! Inserted new point: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
	}

	// corner 2 logic
	ECBPolyPoint corner2Converted(in_corner2.x, in_corner2.y, in_corner2.z);
	ECBPPOrientationResults corner2Orientation = IndependentUtils::getDynamicPointOrientation(corner2Converted, sliceDynBorderRef);
	RCollisionPoint* corner2PointRef = pointArrayRef->attemptPointInsert(in_corner2, corner2Orientation);
	typicalPointSetB[currentPointIndex++] = corner2PointRef;
	std::cout << "!! Inserted corner point 2: " << in_corner1.x << ", " << in_corner1.y << ", " << in_corner1.z << std::endl;

	// any points between corner 2 and corner corner 3 (difference in poinst between these corners is +y)
	float corner2BasedCurrentY = in_corner2.y;
	float corner2PointZ = in_corner2.z;
	//float corner2YIncrementValue = float(yLimitMax - yLimitMin) / float(in_numberOfPointsBetweenCorners + 1);
	float corner2YIncrementValue = dimLimit / float(in_numberOfPointsBetweenCorners + 1);
	for (int x = 0; x < in_numberOfPointsBetweenCorners; x++)
	{
		corner2BasedCurrentY += corner2YIncrementValue;
		glm::vec3 glmPoint(pointX, corner2BasedCurrentY, corner2PointZ);
		ECBPolyPoint newPoint(pointX, corner2BasedCurrentY, corner2PointZ);
		ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(newPoint, sliceDynBorderRef);
		RCollisionPoint* currentPointRef = pointArrayRef->attemptPointInsert(glmPoint, currentPointOrientation);
		typicalPointSetB[currentPointIndex++] = currentPointRef;
		std::cout << "!! Inserted new point: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
	}

	// corner 3 logic
	ECBPolyPoint corner3Converted(in_corner3.x, in_corner3.y, in_corner3.z);
	ECBPPOrientationResults corner3Orientation = IndependentUtils::getDynamicPointOrientation(corner3Converted, sliceDynBorderRef);
	RCollisionPoint* corner3PointRef = pointArrayRef->attemptPointInsert(in_corner3, corner3Orientation);
	typicalPointSetB[currentPointIndex++] = corner3PointRef;
	std::cout << "!! Inserted corner point 3: " << in_corner3.x << ", " << in_corner3.y << ", " << in_corner3.z << std::endl;

	// any points between corner 3 and corner 0 (difference in points beween these corners is -z)
	float corner3BasedCurrentZ = in_corner3.z;
	float corner3PointY = in_corner3.y;
	//float corner3ZDecrementValue = float(zLimitMax - zLimitMin) / float(in_numberOfPointsBetweenCorners + 1);
	float corner3ZDecrementValue = dimLimit / float(in_numberOfPointsBetweenCorners + 1);
	for (int x = 0; x < in_numberOfPointsBetweenCorners; x++)
	{
		corner3BasedCurrentZ -= corner3ZDecrementValue;
		glm::vec3 glmPoint(pointX, corner3PointY, corner3BasedCurrentZ);
		ECBPolyPoint newPoint(pointX, corner3PointY, corner3BasedCurrentZ);
		ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(newPoint, sliceDynBorderRef);
		RCollisionPoint* currentPointRef = pointArrayRef->attemptPointInsert(glmPoint, currentPointOrientation);
		typicalPointSetB[currentPointIndex++] = currentPointRef;
		std::cout << "!! Inserted new point: " << newPoint.x << ", " << newPoint.y << ", " << newPoint.z << std::endl;
	}
}

void RAdditiveXSliceEastEnd::buildPTriangles()
{

}