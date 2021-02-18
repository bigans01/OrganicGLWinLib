#include "stdafx.h"
#include "QMVec3FindOrientatedLineNormal.h"

glm::vec3 QMVec3FindOrientatedLineNormal::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	glm::vec3 returnNormal;

	// check for point translation
	glm::vec3 pointACopy = in_quatRotationPointsRef->getPointByIndex(0);
	glm::vec3 pointBCopy = in_quatRotationPointsRef->getPointByIndex(1);
	glm::vec3 centroidPointCopy = in_quatRotationPointsRef->getPointByIndex(2);

	PointTranslationCheck pointCheck;
	pointCheck.performCheck(pointACopy);
	if (pointCheck.requiresTranslation == 1)
	{
		glm::vec3 translationValue = pointCheck.getTranslationValue();
		pointACopy += translationValue;
		pointBCopy += translationValue;
		centroidPointCopy += translationValue;
	}

	QuatRotationPoints rotationPoints;	
	// NOTE: the centroid value gets replaced with the centroid-facing normal, when
	// runExecutionsForFindingCentroidFacingNormal() is called.
	rotationPoints.insertPointRefs(&pointACopy, &pointBCopy, &centroidPointCopy);
	runExecutionsForFindingCentroidFacingNormal(&rotationPoints);
	return centroidPointCopy;
}

void QMVec3FindOrientatedLineNormal::runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints)
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
		qmVec3BaseLogger.log("(QMBoolPointWithinTrianglePBZ) Centroid is greater than Y! ", "\n");
		determinedEmptyNormal.y = 1;
	}
	else if (currentCentroid.y < 0)
	{
		qmVec3BaseLogger.log("(QMBoolPointWithinTrianglePBZ) Centroid is less than Y! ", "\n");
		determinedEmptyNormal.y = -1;
	}

	glm::vec3* centroidRef = rotationpointsRefVector->getPointRefByIndex(2);
	*centroidRef = determinedEmptyNormal;

	rotatePointsToOriginalPosition(&rotationRecords, rotationpointsRefVector);
}

void QMVec3FindOrientatedLineNormal::rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
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

void QMVec3FindOrientatedLineNormal::rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
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

void QMVec3FindOrientatedLineNormal::rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
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

float QMVec3FindOrientatedLineNormal::getRadiansForRotateToPosYViaX(glm::vec3 in_vec3)
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