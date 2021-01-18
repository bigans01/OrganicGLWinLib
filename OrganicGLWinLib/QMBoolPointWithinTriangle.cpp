#include "stdafx.h"
#include "QMBoolPointWithinTriangle.h"

bool QMBoolPointWithinTriangle::solve(QuatRotationPoints* in_quatRotationPointsRef)
{
	bool returnValue = false;
	bool planeArrayCheckResult[3];
	struct TriangleLineSpec
	{
		ThreeDLineSegment segment;
		glm::vec3 centroidFacingNormal;
	};

	glm::vec3 pointToCheckRoot = in_quatRotationPointsRef->getPointByIndex(0);
	glm::vec3 trianglePoint0 = in_quatRotationPointsRef->getPointByIndex(1);
	glm::vec3 trianglePoint1 = in_quatRotationPointsRef->getPointByIndex(2);
	glm::vec3 trianglePoint2 = in_quatRotationPointsRef->getPointByIndex(3);

	ThreeDLineSegment threeDline1(trianglePoint0, trianglePoint1);
	TriangleLineSpec line1;
	line1.segment = threeDline1;

	ThreeDLineSegment threeDline2(trianglePoint1, trianglePoint2);
	TriangleLineSpec line2;
	line2.segment = threeDline2;

	ThreeDLineSegment threeDline3(trianglePoint2, trianglePoint0);
	TriangleLineSpec line3;
	line3.segment = threeDline3;

	TriangleLineSpec lines[3];
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;

	// first, find the centroid-facing normals for each line
	glm::vec3 centroid = findTriangleCentroid(lines[0].segment.pointA, lines[1].segment.pointA, lines[2].segment.pointA);
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 centroidPointCopy = centroid;

		// first, check for point translation
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			centroidPointCopy += translationValue;
		}

		//std::cout << ">>>>>: point A Copy, post translate: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << std::endl;
		//std::cout << ">>>>>: point B Copy, post translate: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << std::endl;
		//std::cout << ">>>>>: centroid copy, post translate: " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&centroidPointCopy);
		runExecutionsForFindingCentroidFacingNormal(&rotationPoints);


		//std::cout << ">>>>>: centroid copy, post translate (2): " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		lines[x].centroidFacingNormal = centroidPointCopy;
	}

	// now that the normals have been determined, used them for the planarity tests.
	for (int x = 0; x < 3; x++)
	{

		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 pointToCheck = pointToCheckRoot;
		glm::vec3 centroidFacingNormal = lines[x].centroidFacingNormal;

		//std::cout << ">>> pushing back this value for centroid facing normal: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;

		// first, check for point translation; only translate pointA/B, and the point to check against.
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			pointToCheck += translationValue;
		}

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&pointToCheck);
		rotationPoints.pointsRefVector.push_back(&centroidFacingNormal);		// fetched by rotationManager, to check for flipping on Z-axis
		planeArrayCheckResult[x] = runTriangleLinePlanarityTestForPoint(&rotationPoints);

	}

	int withinPlaneCount = 0;
	for (int x = 0; x < 3; x++)
	{
		if (planeArrayCheckResult[x] == true)
		{
			//std::cout << "!!! found as being within plane! " << std::endl;
			withinPlaneCount++;
		}
	}

	if (withinPlaneCount == 3)
	{
		//std::cout << "OrganicGLWinUtils::checkIfPointLiesWithinTriangle --> Point is WITHIN triangle. " << std::endl;
		returnValue = true;
	}
	return returnValue;
}

void QMBoolPointWithinTriangle::runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints)
{
	QuatRotationPoints* rotationpointsRefVector = in_quatRotationPoints;
	glm::vec3* pointBRef = rotationpointsRefVector->getPointRefByIndex(1);
	glm::vec3* pointCRef = rotationpointsRefVector->getPointRefByIndex(2);
	std::vector<QuatRotationType> rotationOrder;
	std::stack<QuatRotationRecord> rotationRecords;

	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane. (NOPE!)
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType);
	}

	rotateLineToZPlane(pointBRef, &rotationRecords, in_quatRotationPoints, &rotationOrder);

	// now, check if the centroid point is at positive y; if at's 0 rotate to positive y.
	if (pointCRef->y == 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		glm::vec3 currentNormalValue = *rotationpointsRefVector->getPointRefByIndex(2);	// get a copy of the value of the 3rd primal point
		float radiansToRotateBy = getRadiansForRotateToPosYViaX(currentNormalValue);	// get the number of radians to rotate by
		//std::cout << "::> radiansToRotateBy " << radiansToRotateBy << std::endl;

		//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
		glm::vec3 rotateAroundX;
		rotateAroundX.x = 1.0f;
		QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

		glm::quat fractureQuat = s1record.returnOriginalRotation();
		rotationpointsRefVector->applyQuaternion(fractureQuat);	// rotate all values by this one
		rotationRecords.push(s1record);
	}

	// check if the 3rd point (the centroid of the triangle is positive Y or negative Y; it's Y should never be 0, if we did things correctly (the triangle would be invalid).
	glm::vec3 determinedEmptyNormal;
	glm::vec3 currentCentroid = rotationpointsRefVector->getPointByIndex(2);
	if (currentCentroid.y > 0)
	{
		determinedEmptyNormal.y = 1;
	}
	else if (currentCentroid.y < 0)
	{
		determinedEmptyNormal.y = -1;
	}

	glm::vec3* centroidRef = rotationpointsRefVector->getPointRefByIndex(2);
	*centroidRef = determinedEmptyNormal;

	rotatePointsToOriginalPosition(&rotationRecords, rotationpointsRefVector);
}

bool QMBoolPointWithinTriangle::runTriangleLinePlanarityTestForPoint(QuatRotationPoints* in_quatRotationPoints)
{
	QuatRotationPoints* rotationpointsRefVector = in_quatRotationPoints;
	glm::vec3* pointBRef = rotationpointsRefVector->getPointRefByIndex(1);
	glm::vec3* pointCRef = rotationpointsRefVector->getPointRefByIndex(3);
	std::vector<QuatRotationType> rotationOrder;
	std::stack<QuatRotationRecord> rotationRecords;

	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane. (NOPE!)
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType);
	}

	/*
	// otherwise, if it's already on Y = 0, do this:
	if (pointBRef->y == 0.0f)
	{
		glm::vec3 centroid = in_quatpointsRefVector->getPointByIndex(2);
		glm::vec3* centroidRef = in_quatpointsRefVector->getPointRefByIndex(2);
		glm::vec3 emptyNormal;

		if (centroid.y < 0)	// it's going negative y
		{
			emptyNormal.y = -1;
		}
		if (centroid.y > 0)
		{
			emptyNormal.y = 1;
		}

		*centroidRef = emptyNormal;
	}
	*/

	//std::cout << "#######> Pre-rotate point values: " << std::endl;
	//rotationpointsRefVector->printPoints();
	rotateLineToZPlane(pointBRef, &rotationRecords, in_quatRotationPoints, &rotationOrder);

	//std::cout << "#######> Post-rotate point values: " << std::endl;
	//rotationpointsRefVector->printPoints();

	/*
	std::cout << "Point C ref, y value is: " << pointCRef->y << std::endl;
	if (pointCRef->y <= 0.0f)
	{
		std::cout << "Point C ref is <= 0!!! " << std::endl;
	}
	*/

	// now, check if the centroid-facing normal point is at positive y; if it isn't, we must rotate to positive y.
	if (pointCRef->y != 1.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		glm::vec3 currentNormalValue = *rotationpointsRefVector->getPointRefByIndex(3);	// get a copy of the value of the centroid-facing normal
		float radiansToRotateBy = getRadiansForRotateToPosYViaX(currentNormalValue);	// get the number of radians to rotate by
		//std::cout << "(**NEW**) ::> radiansToRotateBy " << radiansToRotateBy << std::endl;

		//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
		glm::vec3 rotateAroundX;
		rotateAroundX.x = 1.0f;
		QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

		glm::quat fractureQuat = s1record.returnOriginalRotation();
		rotationpointsRefVector->applyQuaternion(fractureQuat);	// rotate all values by this one
		rotationRecords.push(s1record);
	}


	/*
	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane.
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		std::cout << "(**NEW**) ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}
	*/

	// if the centroid-facing normal is on -y, flip it on the X axis; then compare
	return runRotationsAndPlanarityTest(pointCRef, &rotationRecords, rotationpointsRefVector, &rotationOrder);
}

void QMBoolPointWithinTriangle::rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	auto vectorBegin = (*in_rotationOrderVectorRef).begin();
	auto vectorEnd = (*in_rotationOrderVectorRef).end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateLineAroundZToYZero(in_pointToRotateFor, in_quatRotationRecordStackRef, in_quatRotationPointsRef);
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//rotateAroundYToPosXAndPushIntoStack();
			rotateLineAroundYToPosXAndPushIntoStack(in_pointToRotateFor, in_quatRotationRecordStackRef, in_quatRotationPointsRef);
		}
	}
}

void QMBoolPointWithinTriangle::rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	float atan2result = atan2(in_pointToRotateFor->y, in_pointToRotateFor->x); // find the radians we'll need to rotate by
	//std::cout << "!!! Atan2result is: " << atan2result << std::endl;
	float firstPassRotateRadians = 0.0f;

	//std::cout << "::: atan2 result is: " << atan2result << std::endl;
	if (atan2result > 0.0)
	{
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	in_quatRotationRecordStackRef->push(s1record);

	//rotationpointsRefVector->printPoints();
	//std::cout << ":::: Radian value is: " << radianValue << std::endl;
}

void QMBoolPointWithinTriangle::rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(in_pointToRotateFor->z, in_pointToRotateFor->x); // find the radians we'll need to rotate by
	float firstPassRotateRadians = 0.0f;
	if (atan2result > 0.0)
	{
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAroundY;
	rotationAroundY.y = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	in_quatRotationRecordStackRef->push(s1record);						// push into the stack
}

float QMBoolPointWithinTriangle::getRadiansForRotateToPosYViaX(glm::vec3 in_vec3)
{

	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)
	//std::cout << ">>>>> vec3 value is: " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//quatRotationManagerLogger.log(">>>>> vec3 value is: ", in_vec3.x, ", ", in_vec3.y, ", ", in_vec3.z, "\n");

	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians = 0.0f;


	//if (debugFlag == 1)
	//{
		//std::cout << "Atan result is: " << atan2result << std::endl;
	//}
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

	glm::vec3 rotationAngle;
	rotationAngle.x = 1.0f;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosY;
	targetPosY.y = 1.0f;
	targetPosY = rotationQuat * targetPosY;

	/*
	if (debugFlag == 1)
	{
		std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
		std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
		std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
		std::cout << ">>>> Attempting second pass rotation... " << std::endl;
	}
	*/

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);

	/*
	if (debugFlag == 1)
	{
		std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	}
	*/
	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;
	glm::quat secondRotationQuat = createQuaternion(secondPassRotateFinal, rotationAngle);
	glm::vec3 finalRotatedVec = secondRotationQuat * in_vec3;
	/*
	if (debugFlag == 1)
	{
		std::cout << "(Z-fracture) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
		std::cout << "(Z-fracture) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
	}
	*/
	degreesToRotateOnX = secondPassRotateFinal;
	return degreesToRotateOnX;
}

bool QMBoolPointWithinTriangle::runRotationsAndPlanarityTest(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	bool isWithinPlane = false;
	auto vectorBegin = (*in_rotationOrderVectorRef).begin();
	auto vectorEnd = (*in_rotationOrderVectorRef).end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateLineAroundZToYZero(in_pointToRotateFor, in_quatRotationRecordStackRef, in_quatRotationPointsRef);
		}
	}

	// check if the normal of the lineOfSightCopy is negative y; flip it on X axis if so.
	if (in_quatRotationPointsRef->getPointByIndex(3).y < 0)
	{
		//std::cout << "!!! Note: Flip on x axis required... " << std::endl;
		flipOnXAxis(in_quatRotationPointsRef);
	}

	//glm::vec3 comparedPointCurrentPosition = rotationpointsRefVector->getPointByIndex(2);
	// if the point being compared against is "on" the plane's defining line, we will need to round. (may need to experiment with hundredths, and thousandths)
	glm::vec3 comparedPointCurrentPosition = OrganicGLWinUtils::roundVec3ToHundredths(in_quatRotationPointsRef->getPointByIndex(2));
	glm::vec3 centroidFacingNormal = in_quatRotationPointsRef->getPointByIndex(3);
	//std::cout << "Compared point: " << comparedPointCurrentPosition.x << ", " << comparedPointCurrentPosition.y << ", " << comparedPointCurrentPosition.z << std::endl;
	//std::cout << "Centroid normal is: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;
	if
	(
		//(rotationpointsRefVector->getPointByIndex(2).y >= 0)
		(comparedPointCurrentPosition.y >= 0)
		&&
		(in_quatRotationPointsRef->getPointByIndex(3).y > 0)
	)
	{
		//std::cout << "!!!! Point is WITHIN triangle! " << std::endl;
		isWithinPlane = true;
	}
	else
	{
		//std::cout << "!!!! Point is not within triangle! " << std::endl;
	}
	return isWithinPlane;
}