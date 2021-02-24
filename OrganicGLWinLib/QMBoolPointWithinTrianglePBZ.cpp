#include "stdafx.h"
#include "QMBoolPointWithinTrianglePBZ.h"

bool QMBoolPointWithinTrianglePBZ::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// set debug level
	qmBoolBaseLogger.setDebugLevel(in_polyDebugLevel);
	qmBoolBaseLoggerDebugLevel = in_polyDebugLevel;

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

		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) ================== post-translate stats, for line ", x, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>>>>: point A Copy, post translate: ", pointACopy.x, ", ", pointACopy.y, ", ", pointACopy.z, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>>>>: point B Copy, post translate: ", pointBCopy.x, ", ", pointBCopy.y, ", ", pointBCopy.z, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>>>>: centroid Copy, post translate: ", centroidPointCopy.x, ", ", centroidPointCopy.y, ", ", centroidPointCopy.z, "\n");

		QuatRotationPoints rotationPoints;
		rotationPoints.insertPointRefs(&pointACopy, &pointBCopy, &centroidPointCopy);
		runExecutionsForFindingCentroidFacingNormal(&rotationPoints);


		//std::cout << ">>>>>: centroid copy, post translate (2): " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		lines[x].centroidFacingNormal = centroidPointCopy;
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>>>>: centroid facing normal, post translate: ", centroidPointCopy.x, ", ", centroidPointCopy.y, ", ", centroidPointCopy.z, "\n");
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

		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) pointACopy value is: ", pointACopy.x, ", ", pointACopy.y, ", ", pointACopy.z, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) pointBCopy value is: ", pointBCopy.x, ", ", pointBCopy.y, ", ", pointBCopy.z, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) pointToCheck value is: ", pointToCheck.x, ", ", pointToCheck.y, ", ", pointToCheck.z, "\n");
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) centroidFacingNormal value is: ", centroidFacingNormal.x, ", ", centroidFacingNormal.y, ", ", centroidFacingNormal.z, "\n");
	
		rotationPoints.insertPointRefs(&pointACopy, &pointBCopy, &pointToCheck, &centroidFacingNormal); // centroidFacingNormal is fetched by rotationManager, to check for flipping on Z-axis
		planeArrayCheckResult[x] = runTriangleLinePlanarityTestForPoint(&rotationPoints);

	}

	int withinPlaneCount = 0;
	for (int x = 0; x < 3; x++)
	{
		if (planeArrayCheckResult[x] == true)
		{
			std::cout << "!!! Plane result " << x << " found as being within plane! " << std::endl;
			qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) !!! found as being within plane! ", "\n");
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

void QMBoolPointWithinTrianglePBZ::runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints)
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

	// At this point in time, the line should exist entirely on the ZY plane (X will be near/almost 0.0f)
	// The centroid must be in a position where it is also within the ZY plane. If it is X= 0, that's fine; otherwise,
	// rotate to positive Y.
	//if (pointCRef->y == 0.0f)
	if (pointCRef->x != 0.0f)		// when rotated towards positive or negative y, the x value should be 0.
	{

		// rotate around X to positive y.
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		glm::vec3 currentCentroidValue = *rotationpointsRefVector->getPointRefByIndex(2);	// get a copy of the value of the 3rd primal point
		float radiansToRotateBy = getRadiansForRotateToPosYViaX(currentCentroidValue);	// get the number of radians to rotate by
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
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) Centroid is greater than Y! ", "\n");
		determinedEmptyNormal.y = 1;
	}
	else if (currentCentroid.y < 0)
	{
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) Centroid is less than Y! ", "\n");
		determinedEmptyNormal.y = -1;
	}

	glm::vec3* centroidRef = rotationpointsRefVector->getPointRefByIndex(2);
	*centroidRef = determinedEmptyNormal;

	rotatePointsToOriginalPosition(&rotationRecords, rotationpointsRefVector);
}

bool QMBoolPointWithinTrianglePBZ::runTriangleLinePlanarityTestForPoint(QuatRotationPoints* in_quatRotationPoints)
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


	// At this point in time, the line should exist entirely on the ZY plane (X will be near/almost 0.0f)
	// The centroid must be in a position where it is also within the ZY plane. If it is X= 0, that's fine; otherwise,
	// rotate to positive Y.
	//if (pointCRef->y != 1.0f)
	if (pointCRef->x != 0.0f)		// when rotated towards positive or negative y, the x value should be 0.
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

void QMBoolPointWithinTrianglePBZ::rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
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

void QMBoolPointWithinTrianglePBZ::rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
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

void QMBoolPointWithinTrianglePBZ::rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
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

float QMBoolPointWithinTrianglePBZ::getRadiansForRotateToPosYViaX(glm::vec3 in_vec3)
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

bool QMBoolPointWithinTrianglePBZ::runRotationsAndPlanarityTest(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	/*
	if (qmBoolBaseLogger.isLoggingSet() == true)
	{
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> (****PLANARITY TEST****) starting printing of quat points( ** 2) ", "\n");
		in_quatRotationPointsRef->printPoints();
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> (****PLANARITY TEST****) finished printing of quat points( ** 2)", "\n");
	}
	*/

	bool isWithinPlane = false;

	// unsure what this code block is for, but it's not doing anything good right now. Remove later, if further tests pass without this being used.
	/*
	auto vectorBegin = (*in_rotationOrderVectorRef).begin();
	auto vectorEnd = (*in_rotationOrderVectorRef).end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateLineAroundZToYZero(in_pointToRotateFor, in_quatRotationRecordStackRef, in_quatRotationPointsRef);
		}
	}
	*/

	// check if the normal of the lineOfSightCopy is negative y; flip it on X axis if so.
	if (in_quatRotationPointsRef->getPointByIndex(3).y < 0)
	{
		//std::cout << "!!! Note: Flip on x axis required... " << std::endl;
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> (****PLANARITY TEST****) Flip on X-axis required! ", "\n");
		flipOnXAxis(in_quatRotationPointsRef);
	}

	//glm::vec3 comparedPointCurrentPosition = rotationpointsRefVector->getPointByIndex(2);
	// if the point being compared against is "on" the plane's defining line, we will need to round. (may need to experiment with hundredths, and thousandths)
	//glm::vec3 comparedPointCurrentPosition = OrganicGLWinUtils::roundVec3ToHundredths(in_quatRotationPointsRef->getPointByIndex(2));
	glm::vec3 comparedPointCurrentPosition = roundPointToTenThousandths(in_quatRotationPointsRef->getPointByIndex(2));
	//glm::vec3 comparedPointCurrentPosition = in_quatRotationPointsRef->getPointByIndex(2);

	glm::vec3 centroidFacingNormal = in_quatRotationPointsRef->getPointByIndex(3);
	std::cout << "Compared point: " << comparedPointCurrentPosition.x << ", " << comparedPointCurrentPosition.y << ", " << comparedPointCurrentPosition.z << std::endl;
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
		qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> (****PLANARITY TEST****) Found as being WITHIN plane! ", "\n");
		isWithinPlane = true;
	}
	else
	{
		//std::cout << "!!!! Point is not within triangle! " << std::endl;
	}
	return isWithinPlane;
}

glm::vec3 QMBoolPointWithinTrianglePBZ::roundPointToTenThousandths(glm::vec3 in_vec3)
{
	//in_vec3Ref->x = float(floor(in_vec3Ref->x * 10000 + 0.5) / 10000);
	glm::vec3 returnVec;
	returnVec.x = float(floor(in_vec3.x * 10000 + 0.5) / 10000);
	returnVec.y = float(floor(in_vec3.y * 10000 + 0.5) / 10000);
	returnVec.z = float(floor(in_vec3.z * 10000 + 0.5) / 10000);
	return returnVec;
}