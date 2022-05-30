#include "stdafx.h"
#include "QMFindPointToMassRelationship.h"

PointToMassRelationshipType QMFindPointToMassRelationship::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	PointToMassRelationshipType returnRelationship = PointToMassRelationshipType::NOVAL;

	// set debug level
	qmRelationshipLogger.setDebugLevel(in_polyDebugLevel);
	qmRelationshipLoggerDebugLevel = in_polyDebugLevel;

	// point 0 = point to compare
	// point 1 = point 0 of triangle
	// point 2 = point 1 of triangle
	// point 3 = point 2 of triangle
	// point 4 = the empty normal of the triangle.

	glm::vec3 pointACopy = in_quatRotationPointsRef->getPointByIndex(1);	// get the first point of the triangle, and translate by it.
	PointTranslationCheck pointCheck;
	pointCheck.performCheck(pointACopy);
	if (pointCheck.requiresTranslation == 1)
	{
		glm::vec3 translationValue = pointCheck.getTranslationValue();
		in_quatRotationPointsRef->applyTranslationToIndexRange(translationValue, 0, 3);		// translate all but the last (which is the empty normal)
	}

	// check if the second point of the first line, is aligned such that x > 0, and y = 0.
	// if neither of these are true, we must rotate along the Z axis to positive X.
	std::vector<QuatRotationType> rotationOrder;
	std::stack<QuatRotationRecord> rotationRecords;

	findAxisRotationsToGetFirstLineToXYPlane(in_quatRotationPointsRef, &rotationOrder); // determine how to rotate/translate the first line of the triangle.
	runRotationsToGetLineToXYPlane(in_quatRotationPointsRef, &rotationOrder, &rotationRecords);

	// after the previous two functions, check to see 
	glm::vec3* pointARef = in_quatRotationPointsRef->getPointRefByIndex(1);
	glm::vec3* pointCRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	if
	(
		(pointCRef->z != pointARef->z)		// must be on same Z coordinate
	)
	{
		rotateTriangleAroundX(in_quatRotationPointsRef, &rotationRecords);
	}

	// check whether or not the point to compare, is on the same side as the empty normal.
	glm::vec3* pointToCompareRef = in_quatRotationPointsRef->getPointRefByIndex(0);	// ref to the point to compare.

	// round the Z of the pointToCompare.
	pointToCompareRef->z = roundZValueToThousandths(pointToCompareRef->z);
	if (qmRelationshipLogger.isLoggingSet())
	{
		if (pointToCompareRef->z == 0.0f)
		{
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): pointToCompare's Z is 0!", "\n");
		}
	}

	// assuming the points are in their proper position, determine what to do.
	glm::vec3* emptyNormalRef = in_quatRotationPointsRef->getPointRefByIndex(4);	// ref to the empty normal.
	if (emptyNormalRef->z > 0)	// empty normal would face > 0
	{
		if (pointToCompareRef->z == 0)	// we'd be within the STriangle's plane/triangle, so would be COPLANAR_TO_STRIANGLE
		{
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is COPLANAR_TO_STRIANGLE. ", "\n");
			returnRelationship = PointToMassRelationshipType::COPLANAR_TO_STRIANGLE;
		}

		else if (pointToCompareRef->z < 0)	// we'd be behind the empty normal, so the relationship would be WITHIN_MASS
		{
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is WITHIN_MASS. ", "\n");
			returnRelationship = PointToMassRelationshipType::WITHIN_MASS;
		}
		else if (pointToCompareRef->z > 0)	// we'd be in front of the empty normal, so the relationship would be OUTSIDE_OF_MASS
		{
			
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is OUTSIDE_OF_MASS. ", "\n");
			returnRelationship = PointToMassRelationshipType::OUTSIDE_OF_MASS;
		}
	}
	else if (emptyNormalRef->z < 0)
	{
		if (pointToCompareRef->z == 0)	// we'd be within the STriangle's plane/triangle, so would be COPLANAR_TO_STRIANGLE
		{
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is COPLANAR_TO_STRIANGLE. ", "\n");
			returnRelationship = PointToMassRelationshipType::COPLANAR_TO_STRIANGLE;
		}

		else if (pointToCompareRef->z > 0)	// we'd be behind the empty normal, so the relationship would be WITHIN_MASS
		{
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is WITHIN_MASS. ", "\n");
			returnRelationship = PointToMassRelationshipType::WITHIN_MASS;
		}
		else if (pointToCompareRef->z < 0)   // we'd be in front of the empty normal, so the relationship would be OUTSIDE_OF_MASS
		{
			// not OK, we're outside of the mass
			qmRelationshipLogger.log("(QMFindPointToMassRelationship): Notice, relationship is OUTSIDE_OF_MASS. ", "\n");
			returnRelationship = PointToMassRelationshipType::OUTSIDE_OF_MASS;
		}
	}

	return returnRelationship;
}

void QMFindPointToMassRelationship::findAxisRotationsToGetFirstLineToXYPlane(QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	glm::vec3* pointBRef = in_quatRotationPointsRef->getPointRefByIndex(2);	// grab the second point of the triangle.
	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		qmRelationshipLogger.log("(QMFindPointToMassRelationship): ROTATE_AROUND_Y required. ", "\n");
		//std::cout << "ROTATE_AROUND_Y required." << std::endl;
		in_rotationOrderVectorRef->push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		qmRelationshipLogger.log("(QMFindPointToMassRelationship): ROTATE_AROUND_Z required. ", "\n");
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		in_rotationOrderVectorRef->push_back(rotateType);
	}


}

void QMFindPointToMassRelationship::runRotationsToGetLineToXYPlane(QuatRotationPoints* in_quatRotationPointsRef,
																	std::vector<QuatRotationType>* in_rotationOrderVectorRef,
																	std::stack<QuatRotationRecord>* in_rotationRecordStack)
{
	auto vectorBegin = in_rotationOrderVectorRef->begin();
	auto vectorEnd = in_rotationOrderVectorRef->end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			rotateFirstTriangleLineAroundYAndPushIntoStack(in_quatRotationPointsRef, in_rotationRecordStack);
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateFirstTriangleLineAroundZAndPushIntoStack(in_quatRotationPointsRef, in_rotationRecordStack);
		}
	}
}

void QMFindPointToMassRelationship::rotateFirstTriangleLineAroundYAndPushIntoStack(QuatRotationPoints* in_quatRotationPointsRef,
																					std::stack<QuatRotationRecord>* in_rotationRecordStack)
{
	glm::vec3* pointBRef = in_quatRotationPointsRef->getPointRefByIndex(2);	// we need to get a ref to the second point of the triangle.
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;
	float atan2result = atan2(pointBRef->z, pointBRef->x); // find the radians we'll need to rotate by
	float firstPassRotateRadians = 0.0f;
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

	glm::vec3 rotationAroundY;
	rotationAroundY.y = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one

	glm::vec3* pointBAfterYRotate = in_quatRotationPointsRef->getPointRefByIndex(2);
	qmRelationshipLogger.log("(QMFindPointToMassRelationship): value of pointBAfterYRotate:", pointBAfterYRotate->x, ", ", pointBAfterYRotate->y, ", ", pointBAfterYRotate->z, "\n");

	in_rotationRecordStack->push(s1record);
}

void QMFindPointToMassRelationship::rotateFirstTriangleLineAroundZAndPushIntoStack(QuatRotationPoints* in_quatRotationPointsRef,
																					std::stack<QuatRotationRecord>* in_rotationRecordStack)
{
	glm::vec3* pointBRef = in_quatRotationPointsRef->getPointRefByIndex(2);	// we need to get a ref to the second point of the triangle.

	float radians = 0.0f;
	float fullRadian360 = 6.28319f;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	float atan2result = atan2(pointBRef->y, pointBRef->x); // find the radians we'll need to rotate by
	//std::cout << "!!! Atan2result is: " << atan2result << std::endl;
	float firstPassRotateRadians = 0.0f;

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

	//if (debugFlag == 1)
	//{
		//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
	//}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one

	glm::vec3* pointBAfterZRotate = in_quatRotationPointsRef->getPointRefByIndex(2);
	qmRelationshipLogger.log("(QMFindPointToMassRelationship): value of pointBAfterZRotate:", pointBAfterZRotate->x, ", ", pointBAfterZRotate->y, ", ", pointBAfterZRotate->z, "\n");

	in_rotationRecordStack->push(s1record);
}

void QMFindPointToMassRelationship::rotateTriangleAroundX(QuatRotationPoints* in_quatRotationPointsRef,
															std::stack<QuatRotationRecord>* in_rotationRecordStack)
{
	glm::vec3 currentThirdPoint = in_quatRotationPointsRef->getPointByIndex(3);	// get a copy of the value of the 3rd primal point, which should be at index 3.
	float radiansToRotateBy = findRadiansForRotateTriangleAroundX(currentThirdPoint);


	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	in_quatRotationPointsRef->applyQuaternion(fractureQuat);	// rotate all values by this one
	in_rotationRecordStack->push(s1record);
}

float QMFindPointToMassRelationship::findRadiansForRotateTriangleAroundX(glm::vec3 in_vec3)
{
	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)
	//std::cout << ">>>>> vec3 value is: " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	qmRelationshipLogger.log(">>>>> vec3 value is: ", in_vec3.x, ", ", in_vec3.y, ", ", in_vec3.z, "\n");

	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

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

	//if (debugFlag == 1)
	//{
		//std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
		//std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
		//std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
		//std::cout << ">>>> Attempting second pass rotation... " << std::endl;
	//}

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);

	//if (debugFlag == 1)
	//{
		//std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	//}
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
	//if (debugFlag == 1)
	//{
		//std::cout << "(Z-fracture) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
		//std::cout << "(Z-fracture) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
	//}
	degreesToRotateOnX = secondPassRotateFinal;
	return degreesToRotateOnX;
}

glm::quat QMFindPointToMassRelationship::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}

float QMFindPointToMassRelationship::roundZValueToThousandths(float in_z)
{
	float returnValue;
	returnValue = float(floor(in_z * 1000 + 0.5) / 1000);
	//returnVec.y = float(floor(in_vec3.y * 10000 + 0.5) / 10000);
	//returnVec.z = float(floor(in_vec3.z * 10000 + 0.5) / 10000);
	return returnValue;
}