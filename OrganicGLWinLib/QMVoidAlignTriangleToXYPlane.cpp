#include "stdafx.h"
#include "QMVoidAlignTriangleToXYPlane.h"

void QMVoidAlignTriangleToXYPlane::solve(SPolyScalingOps* in_scalingOpsRef, QuatRotationPoints* in_quatRotationPointsRef)
{
	scalingOpsRef = in_scalingOpsRef;	// set the pointer to the scaling ops.
	quatRotationPointsRef = in_quatRotationPointsRef;	// set pointer to the quat points.

	// NOTE: this QM assumes the triangle has been translated such that the first point of the triangle is at 0,0.

	// set points.
	pointARef = quatRotationPointsRef->getFirstPoint();
	pointBRef = quatRotationPointsRef->getSecondPoint();
	pointCRef = quatRotationPointsRef->getThirdPoint();

	alignFirstLineToXYPlane();
	alignSecondLineToXYPlane();

	std::cout << "+++++++++++++ XZ Plane alignment, points are: " << std::endl;
	quatRotationPointsRef->printPoints();
}

void QMVoidAlignTriangleToXYPlane::alignFirstLineToXYPlane()
{
	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (pointBRef.z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		//std::cout << "ROTATE_AROUND_Y required." << std::endl;
		rotationTypeVector.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (pointBRef.y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationTypeVector.push_back(rotateType);
	}

	auto vectorBegin = rotationTypeVector.begin();
	auto vectorEnd = rotationTypeVector.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Y required, performing... (Z-fracture)" << std::endl;
			//}
			//rotateAroundYAndPushIntoStack();
			rotateFirstLineAroundYAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Z required, performing...(Z-fracture)" << std::endl;
			//}
			rotateFirstLineAroundZAndPushIntoStack();
		}
	}
}

void QMVoidAlignTriangleToXYPlane::alignSecondLineToXYPlane()
{
	if
	(
		(pointCRef.z != pointARef.z)		// must be on same Z coordinate
		//&&									// --AND--
		//(pointCRef->y > 0)					// y for the third point must be positive
	)
	{
		//if (debugFlag == 1)
		//{
			//std::cout << "!!!! Rotation around X required, performing...(Z-fracture)" << std::endl;
		//}
		rotateSecondLineAroundXToXYPlane();
	}
}

void QMVoidAlignTriangleToXYPlane::rotateSecondLineAroundXToXYPlane()
{
	glm::vec3 currentThirdPoint = *quatRotationPointsRef->getThirdPointRef();	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForSecondLine(currentThirdPoint);	// get the number of radians to rotate by

	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	quatRotationPointsRef->applyQuaternion(fractureQuat);	// rotate all values by this one
	scalingOpsRef->rotationRecords.push(s1record);
}

float QMVoidAlignTriangleToXYPlane::findRotationRadiansForSecondLine(glm::vec3 in_vec3)
{
	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)
	//std::cout << ">>>>> vec3 value is: " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//quatRotationManagerLogger.log(">>>>> vec3 value is: ", in_vec3.x, ", ", in_vec3.y, ", ", in_vec3.z, "\n");

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

void QMVoidAlignTriangleToXYPlane::rotateFirstLineAroundYAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;
	float atan2result = atan2(pointBRef.z, pointBRef.x); // find the radians we'll need to rotate by

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
	quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	scalingOpsRef->rotationRecords.push(s1record);						// push into the stack

	//std::cout << "Printing points after Y-axis bound rotation: " << std::endl;
	//rotationpointsRefVector->printPoints();
	//return firstPassRotateRadians;
}

void QMVoidAlignTriangleToXYPlane::rotateFirstLineAroundZAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	
	float atan2result = atan2(pointBRef.y, pointBRef.x); // find the radians we'll need to rotate by

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

	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	scalingOpsRef->rotationRecords.push(s1record);						// push into the stack
}

glm::quat QMVoidAlignTriangleToXYPlane::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}