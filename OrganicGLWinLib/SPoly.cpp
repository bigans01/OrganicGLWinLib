#include "stdafx.h"
#include "SPoly.h"

SPoly::SPoly()
{

};

void SPoly::setEmptyNormal(float in_x, float in_y, float in_z)
{
	glm::vec3 rawEmptyNormal(in_x, in_y, in_z);
	polyEmptyNormal = glm::normalize(rawEmptyNormal);
}

void SPoly::setEmptyNormal(glm::vec3 in_normalVec3)
{
	polyEmptyNormal = glm::normalize(in_normalVec3);
}

void SPoly::setSPolyMaterial(TriangleMaterial in_sPolyMaterialID)
{
	sPolyMaterialID = in_sPolyMaterialID;
}

void SPoly::setBoundaryIndicatorOrientation(BoundaryOrientation in_boundaryOrientation)
{
	sPolyBoundaryIndicator.setBoundaryIndicator(in_boundaryOrientation);
}

void SPoly::setBoundaryIndicator(BoundaryPolyIndicator in_boundaryIndicator)
{
	sPolyBoundaryIndicator = in_boundaryIndicator;
}

BoundaryOrientation SPoly::getBoundaryIndicatorOrientation()
{
	return sPolyBoundaryIndicator.getBoundaryIndicatorValue();
}

std::string SPoly::fetchPrintableBoundaryIndicatorString()
{
	return sPolyBoundaryIndicator.getPrintableIndicatorValue();
}

void SPoly::insertCleaveSequenceAtIndex(int in_indexToInsertAt, CleaveSequence in_cleaveSequence)
{
	std::cout << "(SPoly): inserting CleaveSequence; size of CleaveSequences before insert is: " << cleaveMap.size() << std::endl;
	cleaveMap[in_indexToInsertAt] = in_cleaveSequence;
	std::cout << "(SPoly): Size of CleaveSequences, post insert: " << cleaveMap.size() << std::endl;
}

TriangleMaterial SPoly::getSPolyMaterial()
{
	return sPolyMaterialID;
}

glm::vec3 SPoly::getEmptyNormal()
{
	return polyEmptyNormal;
}

bool SPoly::isSPolyValid()
{
	bool isValid = true;
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		if (trianglesBegin->second.isSTriangleValid() == false)
		{
			isValid = false;	// the first invalid we find, we are done.
			break;				// break out on first contact
		}
	}
	return isValid;
}

bool SPoly::checkForSquarePosZ()
{
	bool checkPassed = false;

	bool point0Found = false;
	bool point1Found = false;
	bool point2Found = false;
	bool point3Found = false;

	auto triangleScanBegin = triangles.begin();
	auto triangleScanEnd = triangles.end();
	for (; triangleScanBegin != triangleScanEnd; triangleScanBegin++)
	{
		for (int x = 0; x < 3; x++)
		{
			glm::vec3 currentPointA = triangleScanBegin->second.triangleLines[x].fetchPointAGlmVec3Version();

			// check for point 0.
			if
			(
				(currentPointA.x == 1.0f)
				&&
				(currentPointA.y == 0.0f)
				&&
				(currentPointA.z == 1.0f)
			)
			{
				//std::cout << "(SPoly::checkForSquarePosZ() -> !!! Found point0. " << std::endl;
				point0Found = true;
			}

			// check for point 1.
			if
			(
				(currentPointA.x == 1.0f)
				&&
				(currentPointA.y == 1.0f)
				&&
				(currentPointA.z == 1.0f)
			)
			{
				//std::cout << "(SPoly::checkForSquarePosZ() -> !!! Found point1. " << std::endl;
				point1Found = true;
			}

			// check for point 2.
			if
			(
				(currentPointA.x == 0.0f)
				&&
				(currentPointA.y == 1.0f)
				&&
				(currentPointA.z == 1.0f)
			)
			{
				//std::cout << "(SPoly::checkForSquarePosZ() -> !!! Found point2. " << std::endl;
				point2Found = true;
			}

			// check for point 3.
			if
			(
				(currentPointA.x == 0.0f)
				&&
				(currentPointA.y == 0.0f)
				&&
				(currentPointA.z == 1.0f)
			)
			{
				//std::cout << "(SPoly::checkForSquarePosZ() -> !!! Found point3. " << std::endl;
				point3Found = true;
			}
		}
	}

	if
	(
		(point0Found == true)
		&&
		(point1Found == true)
		&&
		(point2Found == true)
		&&
		(point3Found == true)
	)
	{
		std::cout << "(SPoly::checkForSquarePosZ): --> NOTICE! found full square poly for POS_Z." << std::endl;
		checkPassed = true;
	}

	return checkPassed;
}

void SPoly::determinePrimalPoints()
{
	primePoint0 = triangles[0].triangleLines[0].fetchPointAGlmVec3Version();
	primePoint1 = triangles[0].triangleLines[1].fetchPointAGlmVec3Version();
	primePoint2 = triangles[0].triangleLines[2].fetchPointAGlmVec3Version();
}

std::map<int, int> SPoly::getBorderLinesForSharedPoint(glm::vec3 in_sharedPoint)
{
	std::map<int, int> returnMap;

	// should always return exactly two values.
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		if
		(
			(borderLinesBegin->second.pointA == in_sharedPoint)
			||
			(borderLinesBegin->second.pointB == in_sharedPoint)
		)
		{
			int currentMapIndex = int(returnMap.size());
			returnMap[currentMapIndex] = borderLinesBegin->first;	// insert the ID of the border line.
		}
	}
	return returnMap;
}

void SPoly::determineBorderLines()
{
	// all SPolys start out in the mode TRIANGLE_FAN
	if (numberOfTriangles == 1)
	{
		addBorderLine(triangles[0].triangleLines[0]);
		addBorderLine(triangles[0].triangleLines[1]);
		addBorderLine(triangles[0].triangleLines[2]);
	}
	else if (numberOfTriangles == 2)
	{
		addBorderLine(triangles[0].triangleLines[0]);
		addBorderLine(triangles[0].triangleLines[1]);
		addBorderLine(triangles[1].triangleLines[1]);
		addBorderLine(triangles[1].triangleLines[2]);
	}
	else
	{
		addBorderLine(triangles[0].triangleLines[0]);
		addBorderLine(triangles[0].triangleLines[1]);
		int lastTriangleIndex = (numberOfTriangles - 1);
		int numberOfIntermediateTriangles = (numberOfTriangles - 2);
		int intermediateStartIndex = 1;
		for (int x = 0; x < numberOfIntermediateTriangles; x++)
		{
			addBorderLine(triangles[intermediateStartIndex++].triangleLines[1]);
		}
		addBorderLine(triangles[lastTriangleIndex].triangleLines[1]);
		addBorderLine(triangles[lastTriangleIndex].triangleLines[2]);
	}

	// after the lines get added, go through each STriangle to have it identify which of its lines are border lines
	for (int x = 0; x < numberOfTriangles; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			glm::vec3 currentPointA = triangles[x].triangleLines[y].fetchPointAGlmVec3Version();
			glm::vec3 currentPointB = triangles[x].triangleLines[y].fetchPointBGlmVec3Version();

			// cycle through each of the border lines
			for (int z = 0; z < numberOfSPolyBorderLines; z++)
			{
				if
					(
					(currentPointA.x == borderLines[z].pointA.x)
						&&
						(currentPointA.y == borderLines[z].pointA.y)
						&&
						(currentPointA.z == borderLines[z].pointA.z)

						&&

						(currentPointB.x == borderLines[z].pointB.x)
						&&
						(currentPointB.y == borderLines[z].pointB.y)
						&&
						(currentPointB.z == borderLines[z].pointB.z)
						)
				{
					triangles[x].triangleLines[y].isBorderLine = 1;	// flag this as being a border line
					triangles[x].triangleLines[y].borderLineID = z;	// set the border line ID
				}

			}
		}
	}
}

void SPoly::setMode(SPolyMode in_mode)
{
	mode = in_mode;
}

SPolyMode SPoly::getMode()
{
	return mode;
}

void SPoly::setMRP(glm::vec3 in_mrp)
{
	massOriginPoint = in_mrp;
}

void SPoly::calculateEmptyNormal()
{
	glm::vec3 point0 = triangles[0].triangleLines[0].fetchPointAGlmVec3Version();
	glm::vec3 point1 = triangles[0].triangleLines[1].fetchPointAGlmVec3Version();
	glm::vec3 point2 = triangles[0].triangleLines[2].fetchPointAGlmVec3Version();

	EmptyNormalFinder finder(point0, point1, point2, massOriginPoint);
	//std::cout << "!!!#####: Value of Empty Normal for this Poly is: " << finder.calculatedNormal.x << ", " << finder.calculatedNormal.y << ", " << finder.calculatedNormal.z << std::endl;
	polyEmptyNormal = finder.calculatedNormal;
}

PerfectClampEnum SPoly::calculateAndGetPerfectClampingValue()
{
	PerfectClampEnum returnValue = PerfectClampEnum::NONE;
	if (triangles.size() != 0)	// will only run if there is at least one triangle to verify.
	{
		returnValue = triangles[0].getPerfectClampingValue();
	}
	return returnValue;
}

void SPoly::determinePlanarVectors()
{
	//std::cout << "#- Planar Vectors       > Calculating planar vectors for the triangles in the SPoly, (" << numberOfSPolyBorderLines << " border lines)... "  << std::endl;
	if (numberOfSPolyBorderLines == 3)
	{
		//if (debugFlag == 1)
		//{
		//std::cout << "(case 1) ~~~~~~~~~ Calculating planar vector for 1 triangle~~~~~~~~~~" << std::endl;
		//}
		findPlanarVectorsForThreeBorderLines(&borderLines[0], &borderLines[1], &borderLines[2]);
	}
	else if (numberOfSPolyBorderLines == 4)
	{
		//if (debugFlag == 1)
		//{
		//std::cout << "(case 2) ~~~~~~~~~ Calculating planar vector for " << numberOfTriangles << " triangles~~~~~~~~~~~~ " << std::endl;
		//}
		//std::cout << "!!!! " << numberOfSPolyBorderLines << std::endl;
		findPlanarVectorsForTwoBorderLines(&borderLines[0], &borderLines[1]);
		findPlanarVectorsForTwoBorderLines(&borderLines[2], &borderLines[3]);
	}
	else
	{
		//if (debugFlag == 1)
		//{
		//std::cout << "(case 3) ~~~~~~~~~ Calculating planar vector for " << numberOfTriangles << " triangles~~~~~~~~~~~~ " << std::endl;
		//}

		//std::cout << "###################### finding Planar vectors for " << numberOfTriangles << " STriangles. " << std::endl;

		glm::vec3 rootPoint = borderLines[0].pointA;
		findPlanarVectorsForTwoBorderLines(&borderLines[0], &borderLines[1]);
		int numberOfIntermediateTriangles = (numberOfTriangles - 2);
		int intermediateStartIndex = 1;
		int intermediateCurrentBorderLine = 2;
		for (int x = 0; x < numberOfIntermediateTriangles; x++)
		{
			findPlanarVectorForSingleBorderLine(rootPoint, &borderLines[intermediateCurrentBorderLine++]);
		}
		findPlanarVectorsForTwoBorderLines(&borderLines[intermediateCurrentBorderLine], &borderLines[intermediateCurrentBorderLine + 1]);

	}
}

void SPoly::findPlanarVectorForSingleBorderLine(glm::vec3 in_triangleFanRootPoint, SPolyBorderLines* in_borderLineOneRef)
{
	glm::vec3 centroidPoint = findCentroid(in_triangleFanRootPoint, in_borderLineOneRef->pointB, in_borderLineOneRef->pointB);
	calculatePlanarVector(in_borderLineOneRef, centroidPoint);
}

void SPoly::findPlanarVectorsForTwoBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef)
{
	if (debugFlag == 1)
	{
		std::cout << "Border Line 1 coords: " << in_borderLineOneRef->pointA.x << ", " << in_borderLineOneRef->pointA.y << ", " << in_borderLineOneRef->pointA.z << std::endl << " | " << in_borderLineOneRef->pointB.x << ", " << in_borderLineOneRef->pointB.y << ", " << in_borderLineOneRef->pointB.z << std::endl;
		std::cout << "Border Line 2 coords: " << in_borderLineTwoRef->pointA.x << ", " << in_borderLineTwoRef->pointA.y << ", " << in_borderLineTwoRef->pointA.z << std::endl << " | " << in_borderLineTwoRef->pointB.x << ", " << in_borderLineTwoRef->pointB.y << ", " << in_borderLineTwoRef->pointB.z << std::endl;
	}
	glm::vec3 centroidPoint = findCentroid(in_borderLineOneRef->pointA, in_borderLineOneRef->pointB, in_borderLineTwoRef->pointB);
	calculatePlanarVector(in_borderLineOneRef, centroidPoint);
	calculatePlanarVector(in_borderLineTwoRef, centroidPoint);

}

void SPoly::findPlanarVectorsForThreeBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef, SPolyBorderLines* in_borderLineThreeRef)
{
	if (debugFlag == 1)
	{
		std::cout << "Finding planar vectors for single triangle..." << std::endl;
	}
	glm::vec3 centroidPoint = findCentroid(in_borderLineOneRef->pointA, in_borderLineTwoRef->pointA, in_borderLineThreeRef->pointA);
	calculatePlanarVector(in_borderLineOneRef, centroidPoint);
	calculatePlanarVector(in_borderLineTwoRef, centroidPoint);
	calculatePlanarVector(in_borderLineThreeRef, centroidPoint);
}

glm::vec3 SPoly::findCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 foundCentroid;
	foundCentroid.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	foundCentroid.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	foundCentroid.z = (in_point0.z + in_point1.z + in_point2.z) / 3;
	//std::cout << "Centroid X: " << foundCentroid.x << std::endl;
	//std::cout << "Centroid y: " << foundCentroid.y << std::endl;
	return foundCentroid;
}

void SPoly::calculatePlanarVector(SPolyBorderLines* in_borderLineRef, glm::vec3 in_centroidPoint)
{
	// NOTE: this function requires all points to be coplanar on Z-plane.
	glm::vec3 pointACopy = in_borderLineRef->pointA;
	glm::vec3 pointBCopy = in_borderLineRef->pointB;
	glm::vec3 centroidPointCopy = in_centroidPoint; // get a copy of the centroid input parameter, so that we may modify it
	if (debugFlag == 1)
	{
		std::cout << "Pre-translate point A: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << ", " << std::endl;
		std::cout << "Pre-translate point B: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << ", " << std::endl;
		std::cout << "Centroid point: " << in_centroidPoint.x << ", " << in_centroidPoint.y << ", " << in_centroidPoint.z << std::endl;
	}

	// first, check for point translation
	PointTranslationCheck pointCheck;
	pointCheck.performCheck(in_borderLineRef->pointA);
	if (pointCheck.requiresTranslation == 1)
	{
		glm::vec3 translationValue = pointCheck.getTranslationValue();
		pointACopy += translationValue;
		pointBCopy += translationValue;
		centroidPointCopy += translationValue;
	}

	// at this point, point A and B of line and the centroid itself should be translated, with point A at 0,0,0. So get the midpoint between A and B, 
	// which is equal to point B / 2.
	if (debugFlag == 1)
	{
		std::cout << "Post-translate (1) point A: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << ", " << std::endl;
		std::cout << "Post-translate (1) point B: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << ", " << std::endl;
		std::cout << "Centroid point (2) : " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;
	}

	// the planar vector should now equal whatever value the centroid is at
	in_borderLineRef->planarVector = QuatUtils::findOrientatedLineNormal(pointACopy, pointBCopy, centroidPointCopy);

	if (debugFlag == 1)
	{
		std::cout << "Post-translate point A: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << ", " << std::endl;
		std::cout << "Post-translate point B: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << ", " << std::endl;
		std::cout << "Post-translate Centroid point: " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		std::cout << "!!! Final calculated planar vector is: " << in_borderLineRef->planarVector.x << ", " << in_borderLineRef->planarVector.y << ", " << in_borderLineRef->planarVector.z << std::endl;
	}
}

glm::vec3 SPoly::rotateForPlanarVectorCalc(glm::vec3* in_pointA, glm::vec3* in_pointB, glm::vec3* in_centroid)
{
	if (debugFlag == 1)
	{
		std::cout << "Point A value: " << in_pointA->x << ", " << in_pointA->y << ", " << in_pointA->z << std::endl;
		std::cout << "Point B value: " << in_pointB->x << ", " << in_pointB->y << ", " << in_pointB->z << std::endl;
		std::cout << "Centroid value: " << in_centroid->x << ", " << in_centroid->y << ", " << in_centroid->z << std::endl;
	}
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;
	float atan2result = atan2(in_pointB->y, in_pointB->x);	// point A should be 0, so compare point B
	float firstPassRotateRadians;

	if (debugFlag == 1)
	{
		std::cout << "!!! Rotation value in radians is: " << atan2result << std::endl;
	}

	if (atan2result > 0.0)
	{
		//firstPassRotateRadians = fullRadian360 - atan2result;
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		//firstPassRotateRadians = abs(atan2result);
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	if (debugFlag == 1)
	{
		std::cout << "##### First Pass rotate radians: " << firstPassRotateRadians << std::endl;
	}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);
	glm::quat originalQuat = s1record.returnOriginalRotation();

	// apply the quat to all 3 points (the two line points, and the centroid)
	*in_pointA = originalQuat * *in_pointA;
	*in_pointB = originalQuat * *in_pointB;
	*in_centroid = originalQuat * *in_centroid;

	if (debugFlag == 1)
	{
		std::cout << "----------------Post original quat rotation: " << std::endl;
		std::cout << "Point A value: " << in_pointA->x << ", " << in_pointA->y << ", " << in_pointA->z << std::endl;
		std::cout << "Point B value: " << in_pointB->x << ", " << in_pointB->y << ", " << in_pointB->z << std::endl;
		std::cout << "Centroid value: " << in_centroid->x << ", " << in_centroid->y << ", " << in_centroid->z << std::endl;
	}
	// determine the vector, store it
	glm::vec3 planarVector;
	if (in_centroid->y < in_pointA->y)	// planar vector was found in -y space
	{
		planarVector.y = -1.0f;
	}
	else if (in_centroid->y > in_pointA->y)
	{
		planarVector.y = 1.0f;
	}

	if (debugFlag == 1)
	{
		std::cout << "!! Planar vector before rotate-back: " << planarVector.x << ", " << planarVector.y << ", " << planarVector.z << std::endl;
	}
	glm::quat reverseQuat = s1record.returnReverseRotation();
	planarVector = reverseQuat * planarVector;

	if (debugFlag == 1)
	{
		std::cout << "!! Planar vector post rotate-back: " << planarVector.x << ", " << planarVector.y << ", " << planarVector.z << std::endl;
	}

	return planarVector;


}

void SPoly::addBorderLine(STriangleLine in_triangleLine)
{
	SPolyBorderLines newBorderLine;
	newBorderLine.pointA = in_triangleLine.fetchPointAGlmVec3Version();
	newBorderLine.pointB = in_triangleLine.fetchPointBGlmVec3Version();
	//std::cout << "!!! Added border line: (" << numberOfSPolyBorderLines << "): " << newBorderLine.pointA.x << ", " << newBorderLine.pointA.y << ", " << newBorderLine.pointA.z << " | " << newBorderLine.pointB.x << ", " << newBorderLine.pointB.y << ", " << newBorderLine.pointB.z << std::endl;
	borderLines[numberOfSPolyBorderLines++] = newBorderLine;
}

int SPoly::getNextBorderLineID(int in_currentBorderLineID, CyclingDirection in_direction)
{
	int returnID = 0;
	//std::cout << "Fetching next border line ID..." << std::endl;


	if (in_direction == CyclingDirection::FORWARD)
	{
		//std::cout << "!! Entered  branch for forward. " << std::endl;
		int lastIndex = numberOfSPolyBorderLines - 1;
		if (in_currentBorderLineID == lastIndex)
		{
			returnID = 0;
		}
		else if (in_currentBorderLineID != lastIndex)
		{
			returnID = in_currentBorderLineID += 1;
		}
	}
	else if (in_direction == CyclingDirection::REVERSE)
	{
		//std::cout << "!! Entered  branch for reverse. " << std::endl;
		int firstIndex = 0;
		if (in_currentBorderLineID == firstIndex)
		{
			returnID = (numberOfSPolyBorderLines - 1);
		}
		else if (in_currentBorderLineID != firstIndex)
		{
			returnID = in_currentBorderLineID -= 1;
		}
	}
	else if (in_direction == CyclingDirection::NOVAL)
	{
		std::cout << "!!! WARNING, no direction detected. " << std::endl;
		int haltVal = 3;
		while (haltVal == 3)
		{

		}
	}
	return returnID;
}

glm::vec3 SPoly::getBorderLineEndpoint(int in_currentBorderLineID, CyclingDirection in_direction)
{
	glm::vec3 returnPoint;

	auto doesIDExist = borderLines.find(in_currentBorderLineID);
	if (doesIDExist == borderLines.end())// wasn't found
	{
		std::cout << "(SPoly): Warning: current border Line ID of " << in_currentBorderLineID << " was not found. " << std::endl;
	}

	if (in_direction == CyclingDirection::FORWARD)
	{
		returnPoint = borderLines[in_currentBorderLineID].pointB;
	}
	else if (in_direction == CyclingDirection::REVERSE)
	{
		returnPoint = borderLines[in_currentBorderLineID].pointA;
	}
	return returnPoint;
}

void SPoly::addTriangle(STriangle in_triangle)
{
	triangles[numberOfTriangles++] = in_triangle;
}

void SPoly::addCategorizedLine(CategorizedLine in_line)
{
	if (in_line.type == IntersectionType::A_SLICE)
	{
		addSlicedCleave(currentCleaveIndex, in_line);
		std::cout << "Adding slice cleave..." << std::endl;
	}
	else
	{
		// add other line types, but check to see if it completes a cleave after the add
		std::cout << "!! No slice detected...performing alternate route" << std::endl;
		//categorizedLineMap[categorizedLineMap.size()] = in_line;	// insert the line
		insertCalibratedLine(in_line);
	}

}

void SPoly::insertCalibratedLine(CategorizedLine in_line)
{
	int currentSize = int(categorizedLineMap.size());
	if (currentSize > 0)			// if it isn't the first one, do this
	{
		// do this if the groupID is the same
		CategorizedLine* previousLine = &categorizedLineMap[int(categorizedLineMap.size()) - 1];
		CategorizedLine candidateLine = in_line;	// the candidate line
		if (previousLine->line.lineGroupID == candidateLine.line.lineGroupID)
		{
			// add a line with the same group ID
			glm::vec3 previousEndpoint = previousLine->line.pointB;
			std::cout << "!!! WARNING, values are: " << std::endl;

			/*
			if (candidateLine.line.pointA != previousEndpoint)
			{
				// perform swapping; grab values for point B
				glm::vec3 swapPoint = candidateLine.line.pointB;
				short swapBorder = candidateLine.line.isPointBOnBorder;
				short swapBorderValue = candidateLine.line.pointBBorder;

				// copy A into B
				candidateLine.line.pointB = candidateLine.line.pointA;
				candidateLine.line.isPointBOnBorder = candidateLine.line.isPointAOnBorder;
				candidateLine.line.pointBBorder = candidateLine.line.pointABorder;

				candidateLine.line.pointA = swapPoint;
				candidateLine.line.isPointAOnBorder = swapBorder;
				candidateLine.line.pointABorder = swapBorderValue;

			}
			*/

			categorizedLineMap[int(categorizedLineMap.size())] = candidateLine;	// insert the newly formed line
		}
		//categorizedLineMap[categorizedLineMap.size()] = in_line;
		else if (previousLine->line.lineGroupID != candidateLine.line.lineGroupID)
		{
			// move the previous group, and THEN add a line with the new group ID
			constructCleaveLine();
			categorizedLineMap[int(categorizedLineMap.size())] = candidateLine;
		}

	}
	else if (currentSize == 0)	// if it is the first one, do this
	{
		categorizedLineMap[int(categorizedLineMap.size())] = in_line;	// insert the line
	}
	//categorizedLineMap[categorizedLineMap.size()] = in_line;	// insert the line
}

void SPoly::addSlicedCleave(int in_currentCleaveIndex, CategorizedLine in_categorizedLine)
{
	//cleaveMap[in_currentCleaveIndex].cleavingLines[0] = in_categorizedLine;
	cleaveMap[in_currentCleaveIndex].insertLineAtIndex(0, in_categorizedLine);
	//cleaveMap[in_currentCleaveIndex].insertNewLine(in_categorizedLine);
	currentCleaveIndex++;
}

int SPoly::checkForNewCleaves()
{
	int returnValue = 0;

	int totalBorderLines = 0;	// the total border lines discovered during this check of the map
	for (int x = 0; x < categorizedLineMap.size(); x++)
	{
		if (categorizedLineMap[x].line.numberOfBorderLines == 1)
		{
			totalBorderLines++;
		}
	}
	std::cout << "Total border lines found: " << totalBorderLines << std::endl;
	if (totalBorderLines == 2)	// if the map currently has two border lines in it, it's technically a cleave.
	{
		returnValue = 1;
	}
	return returnValue;
}

void SPoly::moveLastCleave()
{
	//std::cout << "Checking for last cleave..." << std::endl;
	if (categorizedLineMap.size() != 0)	// are there any lines left?
	{
		//std::cout << "!!!! Lines found to form into a final cleave. " << std::endl;
		/*
		organizeCurrentCleaveLines();	// make sure the lines are in the appropriate order, before moving the last cleave.
		int numberOfLines = categorizedLineMap.size();
		std::cout << "!!!! Number of lines being added: " << numberOfLines << std::endl;
		for (int x = 0; x < numberOfLines; x++)
		{
			cleaveMap[currentCleaveIndex].cleavingLines[x] = categorizedLineMap[x];	// insert the line
		}
		determineCleaveType(currentCleaveIndex);	// always determine the cleave type when adding new cleaves
		std::cout << "Lines inserted into cleaveMap index: " << currentCleaveIndex << std::endl;
		categorizedLineMap.clear();			// wipe out the categorized line map
		*/
		constructCleaveLine();
	}
}

InvalidCleaveSequences SPoly::buildCleaveSequences(CleaveSequenceMergeMode in_cleaveSequenceMergeMode, 
												BoundaryOrientation in_optionalOrientation,
												ExceptionRecorder* in_exceptionRecorderRef)
{
	//std::cout << "############################### BUILDING CLEAVE SEQUENCES ################################### " << std::endl;
	InvalidCleaveSequences retrievedInvalids;
	sequenceFactory.setFactoryBoundaryOrientationOption(in_optionalOrientation);
	if (sequenceFactory.doesFactoryContainLines() == true)	// only do this if there are actually lines to work on
	{	
		//std::cout << "!! Constructing Cleave Sequences. " << std::endl;
		retrievedInvalids = sequenceFactory.constructAndExportCleaveSequences(&cleaveMap, 
																			borderLines, 
																			massManipulationSetting, 
																			in_cleaveSequenceMergeMode,
																			in_exceptionRecorderRef);
		if (retrievedInvalids.containsSequnces == true)
		{
			std::cout << "!!! CleaveSequence construction unsuccessful; preparing to perform alternate action. " << std::endl;
			std::cout << "!!! Border lines of this SPoly are: " << std::endl;
			printBorderLines();

			std::cout << "!!! Points of this SPoly are: " << std::endl;
			printPoints();

			//int alternateWait = 3;
			//std::cin >> alternateWait;
		}
		//std::cout << "!! Done Constructing Cleave Sequences. " << std::endl;
	}
	return retrievedInvalids;
}

void SPoly::constructCleaveLine()
{
	//std::cout << "Cleave is 'CONNECTED' type, continuing..." << std::endl;
	organizeCurrentCleaveLines();
	int numberOfLines = int(categorizedLineMap.size());

	// take all the current categorized lines and form a new cleave line
	for (int x = 0; x < numberOfLines; x++)
	{
		//cleaveMap[currentCleaveIndex].cleavingLines[x] = categorizedLineMap[x];
		cleaveMap[currentCleaveIndex].insertLineAtIndex(x, categorizedLineMap[x]);
	}

	// take the lines from the newly formed cleave line, and check if they intercept any border lines
	int cleaveTypeResult = determineCleaveTypeAndRegisterCatLines(currentCleaveIndex);

	// simple warning, not needed logic.
	if (cleaveTypeResult == 0)
	{
		std::cout << "WARNING, CLEAVE IS DISCONNECTED!!! " << std::endl;
	}

	// remove the categorized lines, since they were just used to produce a new cleave line.

	categorizedLineMap.clear();
	//if (cleaveTypeResult == 1)	// only increment if there was a valid cleave. 
	//{
		//currentCleaveIndex++;
	//}
}

void SPoly::organizeCurrentCleaveLines()
{
	int numberOfLines = int(categorizedLineMap.size());
	if (numberOfLines > 1)		// re-organization should only happen when the number of lines is greater than 1
	{
		std::map<int, CategorizedLine> tempLineMap;		// will store the temp lines into the map
		int firstFoundLineWithBorder = 0;												// Step 1: find the index of the first one to swap out.
		for (int x = 0; x < numberOfLines; x++)
		{
			if (categorizedLineMap[x].line.numberOfBorderLines >= 1)	// find the first index
			{
				firstFoundLineWithBorder = x;
				//std::cout << "^^ First found line with border is: " << x << std::endl;
				break;
			}
		}

		CategorizedLine firstFound = categorizedLineMap[firstFoundLineWithBorder];	// Step 2: get a copy of the first found line, and make sure it's swapped so that point A is the one with the border line.
		firstFound.line.swapBorderToA();											// call this, in case point B (and not A) is on the border.
		glm::vec3 pointToFind = firstFound.line.pointB;								// Step 3: get the point B of our line; search for it in the map
		categorizedLineMap.erase(firstFoundLineWithBorder);								// Step 4: erase the first line we found that we used, from the original map.
		int tempLineMapCurrentIndex = 0;
		tempLineMap[tempLineMapCurrentIndex++] = firstFound;						// Step 5: store the very first line in the new map

		int remainingLineCount = numberOfLines - 1;									// Loop through the remaining original lines
		for (int x = 0; x < remainingLineCount; x++)
		{
			auto mapIter = categorizedLineMap.begin();
			auto mapEnd = categorizedLineMap.end();
			int oldIndexToRemove = 0;						// the index value of the line that will be removed at the end of the loop.
			int removalFlag = 0;							// only remove if this flag is = 1
			for (mapIter; mapIter != mapEnd; mapIter++)
			{
				// use IRPointType result to determine what to do
				IRPointType resultingType = IRPointType::NEITHER;	// make the compiler happy
				resultingType = mapIter->second.checkIfPointIsInLine(pointToFind);
				if (resultingType == IRPointType::POINT_A)	// the point is in the correct position, just add the line, remove it from the old map, and make the new point to find equal to this line's B.
				{
					//std::cout << "CASE, POINT A HIT" << std::endl;
					tempLineMap[tempLineMapCurrentIndex] = mapIter->second;			// insert the line
					oldIndexToRemove = mapIter->first;								// get the old line's index
					pointToFind = tempLineMap[tempLineMapCurrentIndex].line.pointB; // set the new point to find
					tempLineMapCurrentIndex++;										// increment ONLY after we are all done above
					removalFlag = 1;
					break;
				}
				else if (resultingType == IRPointType::POINT_B) // the point was found, but it's backwards. Need to swap it, and then set the values.
				{
					//std::cout << "CASE, POINT B HIT" << std::endl;
					tempLineMap[tempLineMapCurrentIndex] = mapIter->second;			// insert the line
					oldIndexToRemove = mapIter->first;								// get the old line's index						
					//std::cout << "Old values || Point A: " << tempLineMap[tempLineMapCurrentIndex].line.pointA.x << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointA.y << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointA.z << " | " << tempLineMap[tempLineMapCurrentIndex].line.pointB.x << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointB.y << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointB.z << std::endl;
					tempLineMap[tempLineMapCurrentIndex].line.swapToA();		// swap the points
					pointToFind = tempLineMap[tempLineMapCurrentIndex].line.pointB; // set the new point to find
					//std::cout << "New values || Point A: " << tempLineMap[tempLineMapCurrentIndex].line.pointA.x << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointA.y << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointA.z << " | " << tempLineMap[tempLineMapCurrentIndex].line.pointB.x << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointB.y << ", " << tempLineMap[tempLineMapCurrentIndex].line.pointB.z << std::endl;
					tempLineMapCurrentIndex++;
					removalFlag = 1;
					break;
				}
			}
			//std::cout << "!!! map loop complete. " << std::endl;
			if (removalFlag == 1)
			{
				categorizedLineMap.erase(oldIndexToRemove);
			}
		}

		std::cout << "### !!! categorizedLineMap reorganized; Old map size is now: " << categorizedLineMap.size() << std::endl;

		auto tempMapBegin = tempLineMap.begin();
		auto tempMapEnd = tempLineMap.end();
		for (tempMapBegin; tempMapBegin != tempMapEnd; tempMapBegin++)
		{
			int indexToInsert = tempMapBegin->first;
			categorizedLineMap[indexToInsert] = tempMapBegin->second;
		}

		printLines();
		std::cout << "!!!! New organization values: " << std::endl;
	}

}

void SPoly::loadTrianglesAndBorderLinesIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	// load STriangles.
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		for (int x = 0; x < 3; x++)
		{
			in_quatRotationPointsRef->insertPointRefs(trianglesBegin->second.triangleLines[x].getPointARef(),
													  trianglesBegin->second.triangleLines[x].getPointBRef()
													 );
		}
	}

	// load SPolyBorderLines.
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		in_quatRotationPointsRef->insertPointRefs(&borderLinesBegin->second.pointA, &borderLinesBegin->second.pointB);
	}

	// load CleaveSequence data
	sequenceFactory.loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(in_quatRotationPointsRef);
}

void SPoly::loadPrimalsTrianglesAndBordersIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	// load the primals
	in_quatRotationPointsRef->insertPointRefs(&primePoint0, &primePoint1, &primePoint2);

	// load STriangles.
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		// load the lines of each STriangle
		for (int x = 0; x < 3; x++)
		{
			in_quatRotationPointsRef->insertPointRefs(trianglesBegin->second.triangleLines[x].getPointARef(),
				                                      trianglesBegin->second.triangleLines[x].getPointBRef()
														);
		}
	}

	// load SPolyBorderLines.
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		in_quatRotationPointsRef->insertPointRefs(&borderLinesBegin->second.pointA, &borderLinesBegin->second.pointB);
	}
}

void SPoly::loadAllIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	// load the primals
	in_quatRotationPointsRef->insertPointRefs(&primePoint0, &primePoint1, &primePoint2);

	// load STriangles.
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		// load the lines of each STriangle
		for (int x = 0; x < 3; x++)
		{
			in_quatRotationPointsRef->insertPointRefs(trianglesBegin->second.triangleLines[x].getPointARef(),
													  trianglesBegin->second.triangleLines[x].getPointBRef());
		}
	}

	// load SPolyBorderLines.
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		in_quatRotationPointsRef->insertPointRefs(&borderLinesBegin->second.pointA, &borderLinesBegin->second.pointB);
	}

	// load CleaveSequence data
	sequenceFactory.loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(in_quatRotationPointsRef);
}

void SPoly::loadEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	sequenceFactory.loadCategorizedLineEmptyNormalsIntoQuatPoints(in_quatRotationPointsRef);
}

int SPoly::determineCleaveTypeAndRegisterCatLines(int in_cleaveIndexID)
{
	// determine the type here; first get the CleaveSequence
	//std::cout << "!! 1: cleaveMap size before ref: " << cleaveMap.size() << std::endl;
	CleaveSequence* cleaveSequenceRef = &cleaveMap[in_cleaveIndexID];

	//std::cout << "!! 1: cleaveMap size after ref: " << cleaveMap.size() << std::endl;
	int numberOfCategorizedLines = int(cleaveSequenceRef->cleavingLines.size());

	// first, determine the type..CONNECTED or DISCONNECTED
	int currentBorderLineCount = 0;

	// the return value of this function.
	int isConnected = 1;			// default: assume it will be connected.

	// in the below, get the number of border lines from the current categorized line.
	std::cout << "Number of categorized lines: " << numberOfCategorizedLines << std::endl;
	for (int x = 0; x < numberOfCategorizedLines; x++)
	{
		//auto currentCatLine = &cleaveSequenceRef->cleavingLines[x];		// get a ref to the current categorized line
		auto currentCatLine = cleaveSequenceRef->getCategorizedLineRef(x);
		if (currentCatLine->line.numberOfBorderLines > 0)				// register any intercepts
		{
			currentBorderLineCount += currentCatLine->line.numberOfBorderLines;
		}
	}
	if (currentBorderLineCount > 0)		// it is CONNECTED
	{
		for (int x = 0; x < numberOfCategorizedLines; x++)
		{
			//auto currentCatLine = &cleaveSequenceRef->cleavingLines[x];		// get a ref to the current categorized line
			auto currentCatLine = cleaveSequenceRef->getCategorizedLineRef(x);
			if (currentCatLine->line.numberOfBorderLines > 0)				// register any intercepts
			{
				registerIntersectingCatLine(x, *currentCatLine);
			}
		}
		currentCleaveIndex++;
	}

	else if (currentBorderLineCount == 0)	// it is DISCONNECTED
	{
		std::cout << "!!!! WARNING !!!! Disconnected cleave line detected; disregard this cleave line until next pass!!" << std::endl;
		isConnected = 0;		// it is disconnected.
		cleaveMap.erase(in_cleaveIndexID); // remove the referenced map member; referenced members of maps are instantiated if they aren't yet created, so we have to delete it. 
	}


	return isConnected;
}

void SPoly::registerIntersectingCatLine(int lineIndex, CategorizedLine in_line)
{
	// determine how we will insert
	if (in_line.line.numberOfBorderLines == 1)	// one border line
	{
		if (in_line.line.isPointAOnBorder == 1)	// work with point A
		{
			insertIntersectingCatLine(in_line.line.pointABorder, in_line.line.lineGroupID, lineIndex, IRPointType::POINT_A, in_line.line.pointA);	// register point A
		}
		else if (in_line.line.isPointBOnBorder == 1)
		{
			insertIntersectingCatLine(in_line.line.pointBBorder, in_line.line.lineGroupID, lineIndex, IRPointType::POINT_B, in_line.line.pointB);	// " " B
		}
	}

	else if (in_line.line.numberOfBorderLines == 2)
	{
		// register both
		std::cout << "!! CONNECTED LINE: NUmber of border lines is equal to 2!" << std::endl;
		insertIntersectingCatLine(in_line.line.pointABorder, in_line.line.lineGroupID, lineIndex, IRPointType::POINT_A, in_line.line.pointA);
		insertIntersectingCatLine(in_line.line.pointBBorder, in_line.line.lineGroupID, lineIndex, IRPointType::POINT_B, in_line.line.pointB);
	}
}

void SPoly::insertIntersectingCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, IRPointType point_type, glm::vec3 in_point)
{
	intercepts.insertCatLine(in_borderLineID, in_catLineGroupID, in_catLineID, point_type, in_point);
}

void SPoly::printLines()
{
	int cleaveCheck = checkForNewCleaves();						// are there new cleaves to process?
	if (cleaveCheck == 1)
	{
		std::cout << "Performing cleaving logic...analyzing lines: " << std::endl;
		for (int x = 0; x < categorizedLineMap.size(); x++)
		{
			std::cout << x << ": " << categorizedLineMap[x].line.pointA.x << ", " << categorizedLineMap[x].line.pointA.y << "," << categorizedLineMap[x].line.pointA.z << " | " << categorizedLineMap[x].line.pointB.x << ", " << categorizedLineMap[x].line.pointB.y << ", " << categorizedLineMap[x].line.pointB.z << std::endl;
		}
	}
}

void SPoly::printPoints()
{
	for (int x = 0; x < numberOfSPolyBorderLines; x++)
	{
		std::cout << "Point " << x << ": " << borderLines[x].pointA.x << ", " << borderLines[x].pointA.y << ", " << borderLines[x].pointA.z << std::endl;
	}
}

void SPoly::printBorderLines()
{
	for (int x = 0; x < numberOfSPolyBorderLines; x++)
	{
		std::cout << "|||| Border Line " << x << std::endl;
		std::cout << "| point A: " << borderLines[x].pointA.x << ", " << borderLines[x].pointA.y << ", " << borderLines[x].pointA.z << std::endl;
		std::cout << "| point B: " << borderLines[x].pointB.x << ", " << borderLines[x].pointB.y << ", " << borderLines[x].pointB.z << std::endl;
	}
}

void SPoly::printPlanarVectors()
{
	std::cout << "|||| Printing planar vectors for border lines... " << std::endl;
	for (int x = 0; x < numberOfSPolyBorderLines; x++)
	{
		std::cout << "|||| Border Line " << x << std::endl;
		//std::cout << "| point A: " << borderLines[x].pointA.x << ", " << borderLines[x].pointA.y << ", " << borderLines[x].pointA.z << std::endl;
		//std::cout << "| point B: " << borderLines[x].pointB.x << ", " << borderLines[x].pointB.y << ", " << borderLines[x].pointB.z << std::endl;
		std::cout << "| Planar vector: " << borderLines[x].planarVector.x << ", " << borderLines[x].planarVector.y << ", " << borderLines[x].planarVector.z << std::endl;
	}
}

void SPoly::printTriangleMetadata()
{
	std::cout << "SPoly: printing out triangle metadata... " << std::endl;
	std::cout << "Empty normal: " << polyEmptyNormal.x << ", " << polyEmptyNormal.y << ", " << polyEmptyNormal.z << std::endl;
	std::cout << "Indicator data: " << fetchPrintableBoundaryIndicatorString() << std::endl;


	for (auto& currentTriangle : triangles)
	{
		currentTriangle.second.printPoints();
	}
}

void SPoly::printAllCleaveLines()
{
	std::cout << "$$$ beginning cleave line print " << std::endl;
	auto cleaveMapBegin = cleaveMap.begin();
	auto cleaveMapEnd = cleaveMap.end();
	if (cleaveMap.size() > 0)
	{
		for (cleaveMapBegin; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
		{
			std::cout << "####### Printing cleave line " << std::endl;
			int currentCleaveIndex = cleaveMapBegin->first;
			auto currentLinesBegin = cleaveMapBegin->second.cleavingLines.begin();
			auto currentLinesEnd = cleaveMapBegin->second.cleavingLines.end();
			for (currentLinesBegin; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
			{
				std::cout << "[" << currentCleaveIndex << "] [" << currentLinesBegin->first << "] | Point A: (Group " << currentLinesBegin->second.line.lineGroupID << "): " << currentLinesBegin->second.line.pointA.x << ", " << currentLinesBegin->second.line.pointA.y << ", " << currentLinesBegin->second.line.pointA.z << " | Point B "
					<< currentLinesBegin->second.line.pointB.x << ", " << currentLinesBegin->second.line.pointB.y << ", " << currentLinesBegin->second.line.pointB.z << " | Empty Normal: " << currentLinesBegin->second.emptyNormal.x << ", " << currentLinesBegin->second.emptyNormal.y << ", " << currentLinesBegin->second.emptyNormal.z << std::endl;

				// check the types.
				if (currentLinesBegin->second.type == IntersectionType::A_SLICE)
				{
					std::cout << "!!! This line is a SLICE " << std::endl;
					std::cout << "Point A border: " << currentLinesBegin->second.line.pointABorder << std::endl;
					std::cout << "Point B border: " << currentLinesBegin->second.line.pointBBorder << std::endl;
				}

				if (currentLinesBegin->second.type == IntersectionType::PARTIAL_BOUND)
				{
					std::cout << "!!! This line is a PARTIAL_BOUND " << std::endl;
					if (currentLinesBegin->second.line.isPointAOnBorder == 1)
					{
						std::cout << "Point A border: " << currentLinesBegin->second.line.pointABorder << " (Secondary ID: ) " << std::endl;
					}
					if (currentLinesBegin->second.line.isPointBOnBorder == 1)
					{
						std::cout << "Point B border: " << currentLinesBegin->second.line.pointBBorder << " (Secondary ID: ) " << std::endl;
					}
				}
			}
		}
	}
	else if (cleaveMap.empty())
	{
		std::cout << ":::: NO cleave lines to print! :::: " << std::endl;
	}
	int someVal = 4;
	std::cout << "Printing all cleave lines complete, enter number to continue..." << std::endl;
	std::cin >> someVal; 

	std::cout << "Testing...." << std::endl;
}

bool SPoly::wasSpecialLogicUsed()
{
	bool used = false;
	auto cleaveMapBegin = cleaveMap.begin();
	auto cleaveMapEnd = cleaveMap.end();
	for (; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
	{
		if (cleaveMapBegin->second.specialLogicUsedFlag == true)
		{
			used = true;
			break;
		}
	}
	return used;
}

void SPoly::setDebugFlag(int in_debugFlagValue)
{
	debugFlag = in_debugFlagValue;
}

CleaveSequenceCandidateListMap SPoly::buildCleaveSequenceCandidateListMap()
{
	CleaveSequenceCandidateListMap returnMap;
	for (int x = 0; x < numberOfSPolyBorderLines; x++)
	{
		//std::cout << "|||| Candidate list for border line " << x << ": " << std::endl;
		SPolyBorderLines* borderLineRef = &borderLines[x];
		auto recordsBegin = borderLineRef->intersectRecorder.records.begin();
		auto recordsEnd = borderLineRef->intersectRecorder.records.end();
		//candidateListMap.candidateMap[x].
		for (; recordsBegin != recordsEnd; recordsBegin++)
		{
			//std::cout << "> " << recordsBegin->first << std::endl;
			//candidateListMap.candidateMap[x].insertCandidate(recordsBegin->first);
			returnMap.candidateMap[x].insertCandidate(recordsBegin->first);
		}
	}

	//std::cout << "!!!! Size of return map is : -> " << returnMap.candidateMap.size() << std::endl;

	return returnMap;
}

CleaveSequenceMetaTracker SPoly::buildCleaveSequenceMetaTracker()
{
	CleaveSequenceMetaTracker returnTracker;
	auto cleaveBegin = cleaveMap.begin();
	auto cleaveEnd = cleaveMap.end();
	for (; cleaveBegin != cleaveEnd; cleaveBegin++)
	{
		CleaveSequenceMeta currentMeta = cleaveBegin->second.getCleaveSequenceMeta();
		returnTracker.insertNewCleaveSequence(cleaveBegin->first, currentMeta);
	}
	return returnTracker;
}

void SPoly::clearInterceptRegistry()
{
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		borderLinesBegin->second.intersectRecorder.records.clear();
	}
}

void SPoly::applyDebugOptions(SPolyDOSet in_sPolyDOSet)
{
	auto setBegin = in_sPolyDOSet.debugOptions.begin();
	auto setEnd = in_sPolyDOSet.debugOptions.end();
	for (; setBegin != setEnd; setBegin++)
	{
		if (*setBegin == SPolyDO::MAIN)
		{
			std::cout << "!!! ..> debug option for SPolyDO::MAIN encountered, continue? " << std::endl;
			int someVal = 3;
			std::cin >> someVal;
		}
		else if (*setBegin == SPolyDO::FACTORY)
		{
			sequenceFactory.setFactoryDebugLevel(PolyDebugLevel::DEBUG);
		}
		else if (*setBegin == SPolyDO::FACTORY_MERGER)
		{
			sequenceFactory.setMergerDebugLevel(PolyDebugLevel::DEBUG);
		}
	}
}

PointToMassRelationshipMap SPoly::generatePointToMassRelationshipMap()
{
	PointToMassRelationshipMap returnMap;
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		glm::vec3 pointToInsert = borderLinesBegin->second.pointA;
		returnMap.insertNewPoint(pointToInsert);
	}
	return returnMap;
}

BorderLineLinkContainer SPoly::buildBuildBorderLineLinkContainer()
{
	BorderLineLinkContainer returnContainer;
	glm::vec3 pointToCreateLinkFor;
	auto borderLinesBegin = borderLines.begin();
	auto borderLinesEnd = borderLines.end();
	int lastIndex = borderLines.rbegin()->first;		// get the map value of the last border line
	int firstIndex = borderLines.begin()->first;
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		glm::vec3 pointToInsert = borderLinesBegin->second.pointA;
		if (borderLinesBegin->first == firstIndex)	// we're on the first line; the first link would be the last line in the map, second link would be the first line.
		{
			SPolyBorderLines* linkARef = &borderLines.find(lastIndex)->second;
			SPolyBorderLines* linkBRef = &borderLines.find(firstIndex)->second;
			BorderLineLink newLink(pointToInsert, linkARef, linkBRef);
			returnContainer.insertBorderLineLink(newLink);
		}
		else // all other lines, that aren't the first line, simply get the line behind it (whatever occurred previously), as the first link.
		{
			SPolyBorderLines* linkARef = &borderLines.find(borderLinesBegin->first - 1)->second;
			SPolyBorderLines* linkBRef = &borderLines.find(borderLinesBegin->first)->second;
			BorderLineLink newLink(pointToInsert, linkARef, linkBRef);
			returnContainer.insertBorderLineLink(newLink);
		}
	}
	return returnContainer; 
}