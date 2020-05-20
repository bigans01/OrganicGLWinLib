#include "stdafx.h"
#include "QuatRotationManager.h"

void QuatRotationManager::initializeAndRunForEmptyNormal(QuatRotationPoints* in_quatPointsRef)
{
	rotationPointsRef = in_quatPointsRef;
	pointARef = rotationPointsRef->getFirstPointRef();
	pointBRef = rotationPointsRef->getSecondPointRef();
	pointCRef = rotationPointsRef->getThirdPointRef();

	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForEmptyNormal();		// run each rotation against the points in the vector (for empty normal, prior to calculating it)
	calculateEmptyNormal();
	rotateToOriginalPosition();
}

void QuatRotationManager::initializeAndRunForZFracture(QuatRotationPoints* in_quatPointsRef)
{
	rotationPointsRef = in_quatPointsRef;
	pointARef = rotationPointsRef->getFirstPointRef();
	pointBRef = rotationPointsRef->getSecondPointRef();
	pointCRef = rotationPointsRef->getThirdPointRef();

	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForZFracture();
}

CyclingDirection QuatRotationManager::initializeAndRunForCyclingDirectionFinder(QuatRotationPoints* in_quatPointsRef)
{
	rotationPointsRef = in_quatPointsRef;
	CyclingDirection direction = CyclingDirection::NOVAL;	// make the compiler happy

	glm::vec3* borderLinePointARef = rotationPointsRef->getPointRefByIndex(0);
	glm::vec3* borderLinePointBRef = rotationPointsRef->getPointRefByIndex(1);
	pointARef = rotationPointsRef->getPointRefByIndex(2);	// get point A of categorized line
	pointBRef = rotationPointsRef->getPointRefByIndex(3);	// get point B of categorized line
	glm::vec3* categorizedLineEmptyNormal = rotationPointsRef->getPointRefByIndex(4);

	std::cout << "BorderLine Point A ref values: " << borderLinePointARef->x << ", " << borderLinePointARef->y << ", " << borderLinePointARef->z << std::endl;
	std::cout << "BorderLine Point B ref values: " << borderLinePointBRef->x << ", " << borderLinePointBRef->y << ", " << borderLinePointBRef->z << std::endl;
	std::cout << "Categorized Line Point A ref values: " << pointARef->x << ", " << pointARef->y << ", " << pointARef->z << std::endl;
	std::cout << "Categorized Line Point B ref values: " << pointBRef->x << ", " << pointBRef->y << ", " << pointBRef->z << std::endl;
	std::cout << "Categorized Line Empty Normal: " << categorizedLineEmptyNormal->x << ", " << categorizedLineEmptyNormal->y << ", " << categorizedLineEmptyNormal->z << std::endl;

	executeRotationsForCyclingDirectionFinder();
	
	std::cout << "-------------------After quaternion applied: " << std::endl;
	std::cout << "BorderLine Point A ref values: " << borderLinePointARef->x << ", " << borderLinePointARef->y << ", " << borderLinePointARef->z << std::endl;
	std::cout << "BorderLine Point B ref values: " << borderLinePointBRef->x << ", " << borderLinePointBRef->y << ", " << borderLinePointBRef->z << std::endl;
	std::cout << "Categorized Line Point A ref values: " << pointARef->x << ", " << pointARef->y << ", " << pointARef->z << std::endl;
	std::cout << "Categorized Line Point B ref values: " << pointBRef->x << ", " << pointBRef->y << ", " << pointBRef->z << std::endl;
	std::cout << "Categorized Line Empty Normal: " << categorizedLineEmptyNormal->x << ", " << categorizedLineEmptyNormal->y << ", " << categorizedLineEmptyNormal->z << std::endl;

	// find the direction, based on comparing the y's of the borderLine's points and the empty normal of the categorized line.
	float normalizedBorderLinePointAY = borderLinePointARef->y / abs(borderLinePointARef->y);
	float normalizedBorderLinePointBY = borderLinePointBRef->y / abs(borderLinePointBRef->y);
	float normalizedEmptyNormalY = categorizedLineEmptyNormal->y / abs(categorizedLineEmptyNormal->y);

	if (normalizedEmptyNormalY == normalizedBorderLinePointAY)
	{
		std::cout << "Direction is REVERSE" << std::endl;
		direction = CyclingDirection::REVERSE;
	}
	else if (normalizedEmptyNormalY == normalizedBorderLinePointBY)
	{
		std::cout << "Direction is FORWARD" << std::endl;
		direction = CyclingDirection::FORWARD;
	}

	return direction;
	//pointARef = rotationPointsRef->
}

void QuatRotationManager::executeRotationsForEmptyNormal()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			std::cout << "!! Rotation around Y required, performing... " << std::endl;
			rotateAroundYAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			std::cout << "!! Rotation around Z required, performing..." << std::endl;
			rotateAroundZAndPushIntoStack();
		}
	}

	if (pointCRef->y != pointARef->y)		// only rotate around X if point A's y and point C's y aren't equal (otherwise, its where we want it to be, which is all points having same Y)
	{
		std::cout << "!! Rotation around X required, performing..." << std::endl;
		rotateAroundXToYZeroAndPushIntoStack();		// rotate the points so that Y = 0 for the triangle (but the MRP and empty normal's y value will not be 0)
	}
}

void QuatRotationManager::executeRotationsForZFracture()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			if (debugFlag == 1)
			{
				std::cout << "!!!! Rotation around Y required, performing... (Z-fracture)" << std::endl;
			}
			rotateAroundYAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			if (debugFlag == 1)
			{
				std::cout << "!!!! Rotation around Z required, performing...(Z-fracture)" << std::endl;
			}
			rotateAroundZAndPushIntoStack();
		}
	}

	// only rotate around X if these two conditions aren't met (same z, and pointCRef->y veering towards positive1)... otherwise, its where we want it to be, which is all points having same Z --AND-- pointCRef veers towards positive 1
	if 
	(
		(pointCRef->z != pointARef->z)		// must be on same Z coordinate
		&&									// --AND--
		(pointCRef->y > 0)					// y for the third point must be positive
	)
	{
		if (debugFlag == 1)
		{
			std::cout << "!!!! Rotation around X required, performing...(Z-fracture)" << std::endl;
		}
		rotateAroundXForZFractureAndPushIntoStack();
	}
}

void QuatRotationManager::executeRotationsForCyclingDirectionFinder()
{
	if (pointBRef->y != 0)
	{
		rotateAroundZAndPushIntoStack();
	}
}

void QuatRotationManager::calculateEmptyNormal()
{
	glm::vec3 currentPoint0 = *rotationPointsRef->getFirstPointRef();
	glm::vec3 currentPoint1 = *rotationPointsRef->getSecondPointRef();
	glm::vec3 currentPoint2 = *rotationPointsRef->getThirdPointRef();
	glm::vec3 currentMRP = *rotationPointsRef->getMRPRef();

	glm::vec3* currentNormalRef = rotationPointsRef->getNormalRef();
	*currentNormalRef = cross(currentPoint1, currentPoint2);
	std::cout << "##!~~~~~ Points are: " << std::endl;
	std::cout << "0: " << currentPoint0.x << ", " << currentPoint0.y << ", " << currentPoint0.z << std::endl;
	std::cout << "1: " << currentPoint1.x << ", " << currentPoint1.y << ", " << currentPoint1.z << std::endl;
	std::cout << "2: " << currentPoint2.x << ", " << currentPoint2.y << ", " << currentPoint2.z << std::endl;
	std::cout << "MRP: " << currentMRP.x << ", " << currentMRP.y << ", " << currentMRP.z << std::endl;
	std::cout << "##!~~~~~ current normal is: " << currentNormalRef->x << ", " << currentNormalRef->y << ", " << currentNormalRef->z << std::endl;

	glm::vec3 calibratedEmptyNormal = checkForEmptyNormalCorrection(currentMRP, *currentNormalRef);
	*currentNormalRef = calibratedEmptyNormal;

	std::cout << "##!~~~~~ (calibrated) current normal is: " << currentNormalRef->x << ", " << currentNormalRef->y << ", " << currentNormalRef->z << std::endl;

	//int someVal = 3;
	//std::cin >> someVal;
	//rotationPointsRef->pointsRef.push_back(currentNormal);
}

glm::vec3 QuatRotationManager::checkForEmptyNormalCorrection(glm::vec3 in_mrpCopy, glm::vec3 in_normalCopy)
{
	glm::vec3 returnVec = in_normalCopy;
	
	float mrp_y_normalized = in_mrpCopy.y / abs(in_mrpCopy.y);
	float normal_y_normalized = in_normalCopy.y / abs(in_normalCopy.y);

	if (mrp_y_normalized == normal_y_normalized)
	{
		returnVec *= -1;
	}

	return returnVec;
}


glm::vec3 QuatRotationManager::getEmptyNormal()
{
	glm::vec3* currentNormalRef = rotationPointsRef->getNormalRef();
	glm::vec3 returningNormal = *currentNormalRef;
	return returningNormal;
}

void QuatRotationManager::rotateAroundYAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(pointBRef->z, pointBRef->x); // find the radians we'll need to rotate by
	float firstPassRotateRadians;
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
	rotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);						// push into the stack

	//std::cout << "Printing points after Y-axis bound rotation: " << std::endl;
	//rotationPointsRef->printPoints();
	//return firstPassRotateRadians;
}

void QuatRotationManager::rotateAroundZAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	float atan2result = atan2(pointBRef->y, pointBRef->x); // find the radians we'll need to rotate by
	//std::cout << "!!! Atan2result is: " << atan2result << std::endl;
	float firstPassRotateRadians;

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
		std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
	}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//std::cout << "Printing points after Z-axis bound rotation: " << std::endl;
	//rotationPointsRef->printPoints();
}

void QuatRotationManager::rotateAroundXToYZeroAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	float atan2result = atan2(pointCRef->y, pointCRef->z);

	float firstPassRotateRadians;

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
	//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;

	glm::vec3 rotationAroundY;
	rotationAroundY.x = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//std::cout << "Printing points after X-axis bound rotation: (the final rotation?) " << std::endl;
	//rotationPointsRef->printPoints();

}

void QuatRotationManager::rotateAroundXForZFractureAndPushIntoStack()
{
	glm::vec3 currentThirdPoint = *rotationPointsRef->getThirdPointRef();	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForZFracture(currentThirdPoint);	// get the number of radians to rotate by

	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationPointsRef->applyQuaternion(fractureQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//rotationPointsRef->printPoints();
}

float QuatRotationManager::findRotationRadiansForZFracture(glm::vec3 in_vec3)
{

	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)

	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians;


	if (debugFlag == 1)
	{
		std::cout << "Atan result is: " << atan2result << std::endl;
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

	glm::vec3 rotationAngle;
	rotationAngle.x = 1.0f;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosY;
	targetPosY.y = 1.0f;
	targetPosY = rotationQuat * targetPosY;

	if (debugFlag == 1)
	{
		std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
		std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
		std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
		std::cout << ">>>> Attempting second pass rotation... " << std::endl;
	}

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);

	if (debugFlag == 1)
	{
		std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	}
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
	if (debugFlag == 1)
	{
		std::cout << "(Z-fracture) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
		std::cout << "(Z-fracture) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
		std::cout << std::endl;
	}
	degreesToRotateOnX = secondPassRotateFinal;
	return degreesToRotateOnX;
}

glm::quat QuatRotationManager::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}

void QuatRotationManager::rotateToOriginalPosition()
{
	while (!rotationRecords.empty())
	{
		QuatRotationRecord currentRecord = rotationRecords.top();
		std::cout << "################# Reversing values... " << std::endl;
		glm::quat reverseQuat = currentRecord.returnReverseRotation();
		rotationPointsRef->applyQuaternion(reverseQuat);
		rotationPointsRef->printPoints();

		rotationRecords.pop();
	}
}

void QuatRotationManager::setDebugFlag(int in_debugFlag)
{
	debugFlag = in_debugFlag;
}