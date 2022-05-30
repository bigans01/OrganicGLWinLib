#include "stdafx.h"
#include "QMVoidFindCyclingDirection.h"

void QMVoidFindCyclingDirection::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// Logic:
	// To find which cycling direction to choose, compare the non-shared points of of two border lines that do share a point.
	// Whichever point is in the direction of the empty normal, the border line that that point belongs to is the direction we must go in.
	// Before we can compare however, we must use quaternion logic to move the empty normal passed in (at index 3 in the quatRoationPointsRef) to have a value of
	// 0, 1, 0. Once this is done, the points at index 0 and index 2 should have either a negative OR positive y-value. The one that is a positive y value, is the border line
	// we will choose.

	// Note that when solving for a PARTIAL_BOUND line, one of the points will obviously be equal to the shared point. When this happens, the shared point value will get translated to
	// 0,0,0. As a result, this point is never -y or +y, and will be an invalid candidate.

	// set debug level
	qmVoidBaseLogger.setDebugLevel(in_polyDebugLevel);
	qmVoidBaseLoggerDebugLevel = in_polyDebugLevel;

	//QuatRotationPoints* rotationpointsRefVector = in_quatRotationPointsRef;
	rotationpointsRefVector = in_quatRotationPointsRef;
	triangleNormalRef = rotationpointsRefVector->getPointRefByIndex(3);

	qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) !! referenced triangle normal value is: ", triangleNormalRef->x, ", ", triangleNormalRef->y, ", ", triangleNormalRef->z, "\n");
	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (triangleNormalRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		//std::cout << "ROTATE_AROUND_Y required." << std::endl;
		qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) ROTATE_AROUND_Y required, for the CategorizedLine's empty normal.", "\n");
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (triangleNormalRef->x != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		//std::cout << "ROTATE_AROUND_X required." << std::endl;
		qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) ROTATE_AROUND_X required, for the CategorizedLine's empty normal.", "\n");
		rotationOrder.push_back(rotateType);
	}

	// if x and z are 0, but the y is negative -1.0f, we still need to get to y = 1.0f.
	//if (triangleNormalRef->y == -1.0f)
	//if (triangleNormalRef->y < 0.0f)
	if (triangleNormalRef->y != 1.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) ROTATE_AROUND_Z required, for the CategorizedLine's empty normal.", "\n");
		rotationOrder.push_back(rotateType);
	}

	runRotations(&rotationOrder);
}


void QMVoidFindCyclingDirection::runRotations(std::vector<QuatRotationType>* in_rotationOrderVecRef)
{
	auto vectorBegin = (*in_rotationOrderVecRef).begin();
	auto vectorEnd = (*in_rotationOrderVecRef).end();
	qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) Executing rotations...", "\n");
	for (; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			rotateEmptyNormalAroundYToPosZ();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_X)
		{
			rotateEmptyNormalAroundXToPosY();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateEmptyNormalAroundZToPosY();
		}
	}
}


void QMVoidFindCyclingDirection::rotateEmptyNormalAroundYToPosZ()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;
	float atan2result = atan2(triangleNormalRef->z, triangleNormalRef->x); // find the radians we'll need to rotate by
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

	// to get to pos z = 1.0f, add 1.57 radians.
	firstPassRotateRadians += ((fullRadian360 / 4) * 3);

	glm::vec3 rotationAroundY;
	rotationAroundY.y = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);						// push into the stack
}

void QMVoidFindCyclingDirection::rotateEmptyNormalAroundXToPosY()
{
	glm::vec3 currentNormalValue = *rotationpointsRefVector->getPointRefByIndex(3);	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForGettingToPosYThroughX(currentNormalValue);	// get the number of radians to rotate by
	//std::cout << "::> radiansToRotateBy " << radiansToRotateBy << std::endl;
	qmVoidBaseLogger.log("(QMVoidFindCyclingDirection) Radians to rotate by to get to positive y is: ", radiansToRotateBy, "\n");

	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);	// rotate all values by this one
	rotationRecords.push(s1record);
}

void QMVoidFindCyclingDirection::rotateEmptyNormalAroundZToPosY()
{
	glm::vec3 currentNormalValue = *rotationpointsRefVector->getPointRefByIndex(3);	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForGettingToPosYThroughZ(currentNormalValue);

	glm::vec3 rotateAroundZ;
	rotateAroundZ.z = -1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundZ);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);
	rotationRecords.push(s1record);
}

float QMVoidFindCyclingDirection::findRotationRadiansForGettingToPosYThroughX(glm::vec3 in_vec3)
{
	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)
	//std::cout << ">>>>> vec3 value is: " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;

	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians = 0.0f;

	//
	//if (debugFlag == 1)
	//{
	//	std::cout << "Atan result is: " << atan2result << std::endl;
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
	//	std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	//	std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
	//	std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
	//	std::cout << ">>>> Attempting second pass rotation... " << std::endl;
	//}
	

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);

	
	//if (debugFlag == 1)
	//{
	//	std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
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
	//	std::cout << "(Z-fracture) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	//	std::cout << "(Z-fracture) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
	//}
	
	degreesToRotateOnX = secondPassRotateFinal;
	return degreesToRotateOnX;
}

float QMVoidFindCyclingDirection::findRotationRadiansForGettingToPosYThroughZ(glm::vec3 in_vec3)
{
	// The overarching goal is to get to POS Y for this 3rd point(3rd point is the value that was passed in), by rotating around the Z axis

	float degreesToRotateOnZ = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.x);
	float firstPassRotateRadians = 0.0f;

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Passed point is:  " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Rotating passed point to Pos X (X = 1.0f), and finding degrees to rotate to get to it... " << std::endl;

	
	//if (debugFlag == 1)
	//{
	//	std::cout << "Atan result is: " << atan2result << std::endl;
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
	rotationAngle.z = -1.0f;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosY;
	targetPosY.y = 1.0f;
	targetPosY = rotationQuat * targetPosY;


	//std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	//std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
	//std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
	//std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosY.y, targetPosY.x);

	
	//if (debugFlag == 1)
	//{
	//	std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
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

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
	//std::cout << std::endl;

	degreesToRotateOnZ = secondPassRotateFinal;

	//int someVal = 3;
	//std::cin >> someVal;


	return degreesToRotateOnZ;
}
