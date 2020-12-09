#include "stdafx.h"
#include "QuatRotationManager.h"

void QuatRotationManager::initializeAndRunForEmptyNormal(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointARef = rotationpointsRefVector->getFirstPointRef();
	pointBRef = rotationpointsRefVector->getSecondPointRef();
	pointCRef = rotationpointsRefVector->getThirdPointRef();

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



void QuatRotationManager::initializeAndRunForZFracture(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointARef = rotationpointsRefVector->getFirstPointRef();
	pointBRef = rotationpointsRefVector->getSecondPointRef();
	pointCRef = rotationpointsRefVector->getThirdPointRef();

	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (pointBRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		//std::cout << "ROTATE_AROUND_Y required." << std::endl;
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForZFracture();
}

void QuatRotationManager::initializeAndRunForFindingBorderLine(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	triangleNormalRef = rotationpointsRefVector->getPointRefByIndex(3);	// the last point in the QuatRotationPoints should equal the empty normal.

	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (triangleNormalRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		//std::cout << "ROTATE_AROUND_Y required." << std::endl;
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (triangleNormalRef->x != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForFindingBorderLine();
}

void QuatRotationManager::initializeAndRunForCyclingDirectionFinderV2(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	triangleNormalRef = rotationpointsRefVector->getPointRefByIndex(3);	// the last point in the QuatRotationPoints should equal the empty normal.

	std::cout << "!!! Finder V2, trianfleNormalRef: " << triangleNormalRef->x << ", " << triangleNormalRef->y << ", " << triangleNormalRef->z << std::endl;

	// check if we need to rotate about the Y-axis to get to the same Z values for the line
	if (triangleNormalRef->z != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		std::cout << "ROTATE_AROUND_Y required." << std::endl;
		rotationOrder.push_back(rotateType); //push into the vector
	}

	// check if we need to rotate about the Z-axis to get to the same Y values for the line
	if (triangleNormalRef->x != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		std::cout << "ROTATE_AROUND_X required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	// if x and z are 0, but the y is negative -1.0f, we still need to get to y = 1.0f.
	//if (triangleNormalRef->y == -1.0f)
	if (triangleNormalRef->y < 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForCyclingDirectionFinderV2();
}

void QuatRotationManager::initializeAndRunForCoplanarCategorizedLineEmptyNormal(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointBRef = in_quatpointsRefVector->getPointRefByIndex(1);

	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane.
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	// otherwise, if it's already on Y = 0, do this:
	if (pointBRef->y == 0.0f)
	{
		glm::vec3 centroid = in_quatpointsRefVector->getPointByIndex(2);
		glm::vec3* emptyNormalRef = in_quatpointsRefVector->getPointRefByIndex(3);
		glm::vec3 emptyNormal;
		if (centroid.y < 0)	// the centroid is in -y, so the empty normal must be opposite
		{
			emptyNormal.y = 1;
		}
		if (centroid.y > 0)	// vice versa
		{
			emptyNormal.y = -1;
		}

		*emptyNormalRef = emptyNormal;
	}

	executeRotationsForFindingCoplanarCategorizedLineEmptyNormal();
	rotateToOriginalPosition();
}

void QuatRotationManager::initializeAndRunForFindingBorderLineEmptyNormal(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointBRef = in_quatpointsRefVector->getPointRefByIndex(1);	
	
	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane.
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

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

	executeRotationsForFindingBorderLineEmptyNormal();
	rotateToOriginalPosition();

	//std::cout << "!!! ~~~~~~ Printing points for test of finding the empty normal for a border line: " << std::endl;
	//rotationpointsRefVector->printPoints();
}

bool QuatRotationManager::initializeAndRunForCheckingIfPointIswithinPlane(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointBRef = in_quatpointsRefVector->getPointRefByIndex(1);

	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane.
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	// if the centroid-facing normal is on -y, flip it on the X axis
	return executeRotationsAndGetResultForCheckingIfPointIswithinPlane();
}

bool QuatRotationManager::executeRotationsAndGetResultForCheckingIfPointIswithinPlane()
{
	bool isWithinPlane = false;
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateAroundZToYZero();
		}
	}

	// check if the normal of the lineOfSightCopy is negative y; flip it on X axis if so.
	if (rotationpointsRefVector->getPointByIndex(3).y < 0)
	{
		//std::cout << "!!! Note: Flip on x axis required... " << std::endl;
		flipOnXAxis();
	}

	glm::vec3 comparedPointCurrentPosition = rotationpointsRefVector->getPointByIndex(2);
	glm::vec3 centroidFacingNormal = rotationpointsRefVector->getPointByIndex(3);
	//std::cout << "Compared point: " << comparedPointCurrentPosition.x << ", " << comparedPointCurrentPosition.y << ", " << comparedPointCurrentPosition.z << std::endl;
	//std::cout << "Centroid normal is: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;
	if
	(
		(rotationpointsRefVector->getPointByIndex(2).y > 0)
		&&
		(rotationpointsRefVector->getPointByIndex(3).y > 0)
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



float QuatRotationManager::initializeAndRunForFindingObserverRadians(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointBRef = in_quatpointsRefVector->getPointRefByIndex(3);

	// check if the slopes are the same; if they are, set a flag to indicate that the degrees will always be 180.
	glm::vec3 observerSlope = rotationpointsRefVector->getPointByIndex(1) - rotationpointsRefVector->getPointByIndex(0);
	glm::vec3 lineOfSightSlope = rotationpointsRefVector->getPointByIndex(3) - rotationpointsRefVector->getPointByIndex(2);

	//std::cout << "-> Observer slope: " << observerSlope.x << ", " << observerSlope.y << ", " << observerSlope.z << std::endl;
	//std::cout << "-> Line of Sight slope: " << lineOfSightSlope.x << ", " << lineOfSightSlope.y << ", " << lineOfSightSlope.z << std::endl;

	//if (observerSlope == lineOfSightSlope)
	//{
	//	std::cout << "!!!!! WARNING: slopes are the same! " << std::endl;
	//}


	// we should only need to check if the y is equal to 0; this function assumes that the SPoly has been aligned to the Z-plane.
	if (pointBRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);
	}

	return executeRotationsForFindingObserverRadians();
}

bool QuatRotationManager::initializeAndRunForCheckingCoplanarity(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	pointBRef = in_quatpointsRefVector->getPointRefByIndex(2);	// get a ref to the point to compare to; we need to rotate this point to 1,0,0. (we should be using the normal.)
	if (pointBRef->x !=	1.0f)
	{
		//std::cout << "::::::: Co-planarity: rotate around Z required. " << std::endl;
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		//std::cout << "ROTATE_AROUND_Z required." << std::endl;
		rotationOrder.push_back(rotateType);		// should call rotateAroundZToYZero();
	}
	if (pointBRef->z != 0.0f)
	{
		//std::cout << "::::::: Co-planarity: rotate around Y required. " << std::endl;
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;		// rotate around Y to x = 1.0f
		rotationOrder.push_back(rotateType);		// should call rotateAroundZToYZero();
	}
	//rotationpointsRefVector->printPoints();

	return executeRotationsForCheckingCoplanarity();
}

bool QuatRotationManager::executeRotationsForCheckingCoplanarity()
{
	bool isCoplanar = false;
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateAroundZToYZero();
		}
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//rotateAroundZToYZero();
		}
	}

	// check if the point we're comparing to (the point in the other SPoly that's being compared to)
	if (rotationpointsRefVector->getPointByIndex(1).y < 0)
	{
		//std::cout << "!!! Note: Flip on x axis required... " << std::endl;
		flipOnXAxis();
	}

	//rotationpointsRefVector->printPoints();
	isCoplanar = checkForRightAngle(rotationpointsRefVector->getPointByIndex(2), rotationpointsRefVector->getPointByIndex(1));

	//std::cout << "::::::::::::: Coplanarity-testing, halt. " << std::endl;
	//int someVal = 5;
	//std::cin >> someVal;

	return isCoplanar;
}

bool QuatRotationManager::checkForRightAngle(glm::vec3 in_pointAtY0, glm::vec3 in_otherPoint)
{
	bool wasRightAngleFound = false;
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	float atan2result = atan2(in_otherPoint.y, in_otherPoint.x); // find the radians we'll need to rotate by
	//std::cout << "!!! Atan2result is: " << atan2result << std::endl;
	float firstPassRotateRadians = 0.0f;

	//std::cout << "::: atan2 result is: " << atan2result << std::endl;

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

	//std::cout << ">>>> Check for right angle radians (pre-round) is: " << firstPassRotateRadians << std::endl;
	firstPassRotateRadians = roundRadiansForRightAngleCheck(firstPassRotateRadians);
	//std::cout << ">>>> Check for right angle radians (post-round) is: " << firstPassRotateRadians << std::endl;
	if (firstPassRotateRadians == 1.5708f)
	{
		wasRightAngleFound = true;
		//std::cout << "!!! Right angle detected. " << std::endl;
	}

	return wasRightAngleFound;
}

float QuatRotationManager::executeRotationsForFindingObserverRadians()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateAroundZToYZero();		
		}
	}

	// check if the normal of the lineOfSightCopy is negative y; flip it on X axis if so.
	if (rotationpointsRefVector->getPointByIndex(5).y < 0)
	{
		//std::cout << "!!! Note: Flip on x axis required... " << std::endl;
		flipOnXAxis();
	}

	return findRadiansForObservation();
}

void QuatRotationManager::rotateAroundZToYZero()
{
	if (pointBRef->y != 0.0f)
	{
		float radians = 0.0f;
		float fullRadian360 = 6.28319;

		//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
		//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
		float atan2result = atan2(pointBRef->y, pointBRef->x); // find the radians we'll need to rotate by
		//std::cout << "!!! Atan2result is: " << atan2result << std::endl;
		float firstPassRotateRadians = 0.0f;

		//std::cout << "::: atan2 result is: " << atan2result << std::endl;

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
			//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
		}
		glm::vec3 rotationAroundZ;
		rotationAroundZ.z = -1.0f;
		QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

		glm::quat originalQuat = s1record.returnOriginalRotation();
		//*pointBRef = originalQuat * *pointBRef;	
		rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
		rotationRecords.push(s1record);

		//std::cout << ":::: Observer calculation: Printing points after Z-axis bound rotation: " << std::endl;
		//rotationpointsRefVector->printPoints();

		//std::cout << ":::: Radian value is: " << radianValue << std::endl;
	}
}

void QuatRotationManager::flipOnXAxis()
{
	glm::vec3 rotationAroundX;
	rotationAroundX.x = -1.0f;
	float rotateRadians = 6.28319 / 2;
	QuatRotationRecord s1record(rotateRadians, rotationAroundX);
	glm::quat originalQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	//std::cout << ":::: Observer calculation: Printing points after X-axis flip: " << std::endl;
	//rotationpointsRefVector->printPoints();
}

float QuatRotationManager::findRadiansForObservation()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	pointARef = rotationpointsRefVector->getFirstPointRef();

	//std::cout << "!! Point A ref X is: " << pointARef->x << std::endl;
	//std::cout << "!! Point A ref Y is: " << pointARef->y << std::endl;

	//std::cout << "=========================== " << std::endl;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;

	//float atan2result = roundToThousandths(atan2(pointARef->y, pointARef->x)); // find the radians we'll need to rotate by
	float atan2result = atan2(pointARef->y, pointARef->x); // find the radians we'll need to rotate by
	float firstPassRotateRadians = 0.0f;

	//std::cout << "::: atan2 result is: " << atan2result << std::endl;

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

	//std::cout << "#### Radian value for observation is: " << firstPassRotateRadians << std::endl;
	radianValue = firstPassRotateRadians;
	return firstPassRotateRadians;
}

float QuatRotationManager::roundToThousandths(float in_float)
{
	float calcedFloat = float(floor(in_float * 1000 + 0.5) / 1000);
	return calcedFloat;
}

float QuatRotationManager::roundRadiansForRightAngleCheck(float in_angleInRadians)
{
	float calcedFloat = float(floor(in_angleInRadians * 10000 + 0.5) / 10000);
	return calcedFloat;
}

void QuatRotationManager::executeRotationsForFindingBorderLine()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Y required, performing... (Planar sliding)" << std::endl;
			//}
			rotateAroundYToPosZForPlanarSlideAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_X)
		{
			//if (debugFlag == 1)
			//{
			//std::cout << "!!!! Rotation around Z required, performing...(Planar sliding)" << std::endl;
			//}
			//rotateAroundZAndPushIntoStack();
			//rotateAroundXToYZeroAndPushIntoStack();
			//rotateAroundXToYZeroForPlanarSlideAndPushIntoStack();
			rotateAroundXForPositiveYBorderLineAndPushIntoStack();
		}
	}


	//std::cout << ":::::: Printingt points for executeRotationsForFindingBorderLine() " << std::endl;
	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::executeRotationsForCyclingDirectionFinderV2()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();

	std::cout << "!! Executing rotations..." << std::endl;
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Y required, performing... (Planar sliding)" << std::endl;
			//}
			rotateAroundYToPosZForPlanarSlideAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_X)
		{
			//if (debugFlag == 1)
			//{
			//std::cout << "!!!! Rotation around Z required, performing...(Planar sliding)" << std::endl;
			//}
			//rotateAroundZAndPushIntoStack();
			//rotateAroundXToYZeroAndPushIntoStack();
			//rotateAroundXToYZeroForPlanarSlideAndPushIntoStack();
			rotateAroundXForPositiveYBorderLineAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//glm::vec3 tempVec = rotationpointsRefVector->getPointByIndex(3);
			//std::cout << "!!! emptyNormal value, prior to rotateAroundZ: " << tempVec.x << ", " << tempVec.y << ", " << tempVec.z << std::endl;
			rotateAroundZForPosYNormalAndPushIntoStack(rotationpointsRefVector->getPointByIndex(3));
		}
	}


	//std::cout << ":::::: Printingt points for executeRotationsForFindingBorderLine() " << std::endl;
	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::executeRotationsForFindingCoplanarCategorizedLineEmptyNormal()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//rotateAroundZToFindBorderLineEmptyNormalAndPushIntoStack();
			rotateAroundZToFindCoplanarCategorizedLineEmptyNormalAndPushIntoStack();
		}
	}
}

void QuatRotationManager::executeRotationsForFindingBorderLineEmptyNormal()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			rotateAroundZToFindBorderLineEmptyNormalAndPushIntoStack();
		}
	}

}

glm::vec3 QuatRotationManager::initializeAndRunForPlanarSlide(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	triangleNormalRef = rotationpointsRefVector->getPointRefByIndex(5);

	// check if we need to rotate about the Y-axis to get to the normal to positive Z.
	if (triangleNormalRef->z != 1.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType);
	}

	// check if we need to rotate about the Z-axis so that the y of the normal is at 0
	if (triangleNormalRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForPlanarSlide();

	// acquire the sliding vector, before we rotate back.
	float slidingVectorZ = rotationpointsRefVector->getPointByIndex(4).z;
	glm::vec3 slidingVector;
	slidingVector.z = slidingVectorZ;
	glm::vec3* slidingVectorRef = rotationpointsRefVector->getPointRefByIndex(6);
	*slidingVectorRef = slidingVector;

	//std::cout << "Sliding vector before reverse rotate is: " << slidingVectorRef->x << ", " << slidingVectorRef->y << ", " << slidingVectorRef->z << std::endl;

	rotateToOriginalPosition();

	//std::cout << "Sliding vector after reverse rotate is: " << slidingVectorRef->x << ", " << slidingVectorRef->y << ", " << slidingVectorRef->z << std::endl;

	return *slidingVectorRef;	// return the calculated sliding vector.

}

void QuatRotationManager::initializeAndRunForPlanarAlignmentToZ(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationOrder.clear();
	rotationpointsRefVector = in_quatpointsRefVector;
	triangleNormalRef = rotationpointsRefVector->getPointRefByIndex(4);

	// check if we need to rotate about the Y-axis to get to the normal to positive Z.
	if (triangleNormalRef->z != 1.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType);
	}

	// check if we need to rotate about the Z-axis so that the y of the normal is at 0
	if (triangleNormalRef->y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		rotationOrder.push_back(rotateType);
	}

	executeRotationsForPlanarSlide();

}

CyclingDirection QuatRotationManager::initializeAndRunForCyclingDirectionFinder(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	CyclingDirection direction = CyclingDirection::NOVAL;	// make the compiler happy

	glm::vec3* borderLinePointARef = rotationpointsRefVector->getPointRefByIndex(0);
	glm::vec3* borderLinePointBRef = rotationpointsRefVector->getPointRefByIndex(1);
	pointARef = rotationpointsRefVector->getPointRefByIndex(2);	// get point A of categorized line
	pointBRef = rotationpointsRefVector->getPointRefByIndex(3);	// get point B of categorized line
	glm::vec3* categorizedLineEmptyNormal = rotationpointsRefVector->getPointRefByIndex(4);

	
	std::cout << "#- Cycling Direction    > (Pre-quat rotation)  | BorderLine Point A ref values " << borderLinePointARef->x << ", " << borderLinePointARef->y << ", " << borderLinePointARef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Pre-quat rotation)  | BorderLine Point B ref values: " << borderLinePointBRef->x << ", " << borderLinePointBRef->y << ", " << borderLinePointBRef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Pre-quat rotation)  | Categorized Line Point A ref values: " << pointARef->x << ", " << pointARef->y << ", " << pointARef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Pre-quat rotation)  | Categorized Line Point B ref values: " << pointBRef->x << ", " << pointBRef->y << ", " << pointBRef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Pre-quat rotation)  | Categorized Line Empty Normal: " << categorizedLineEmptyNormal->x << ", " << categorizedLineEmptyNormal->y << ", " << categorizedLineEmptyNormal->z << std::endl;
	
	rotateEmptyNormalToPosY(categorizedLineEmptyNormal);		// rotate the normal to have Y = 1.0f, so that we may calculate the direction the appropriate way.
	
	
	std::cout << "#- Cycling Direction    > -------------------After quaternions applied (for rotation of empty normal to positive Y): " << std::endl;
	std::cout << "#- Cycling Direction    > (Post-quat rotation) | BorderLine Point A ref values: " << borderLinePointARef->x << ", " << borderLinePointARef->y << ", " << borderLinePointARef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Post-quat rotation) | BorderLine Point B ref values: " << borderLinePointBRef->x << ", " << borderLinePointBRef->y << ", " << borderLinePointBRef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Post-quat rotation) | Categorized Line Point A ref values: " << pointARef->x << ", " << pointARef->y << ", " << pointARef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Post-quat rotation) | Categorized Line Point B ref values: " << pointBRef->x << ", " << pointBRef->y << ", " << pointBRef->z << std::endl;
	std::cout << "#- Cycling Direction    > (Post-quat rotation) | Categorized Line Empty Normal: " << categorizedLineEmptyNormal->x << ", " << categorizedLineEmptyNormal->y << ", " << categorizedLineEmptyNormal->z << std::endl;
	

	// find the direction, based on comparing the y's of the borderLine's points and the empty normal of the categorized line.
	float normalizedBorderLinePointAY = borderLinePointARef->y / abs(borderLinePointARef->y);
	float normalizedBorderLinePointBY = borderLinePointBRef->y / abs(borderLinePointBRef->y);
	float normalizedEmptyNormalY = categorizedLineEmptyNormal->y / abs(categorizedLineEmptyNormal->y);

	std::cout << "|| Value of normalizedEmptyNormalY: " << normalizedEmptyNormalY << std::endl;
	std::cout << "|| Normalized point A Y: " << normalizedBorderLinePointAY << std::endl;
	std::cout << "|| Normalized point B Y: " << normalizedBorderLinePointBY << std::endl;

	if (normalizedEmptyNormalY == normalizedBorderLinePointAY)
	{
		std::cout << "#- Cycling Direction    > Direction is REVERSE, going towards border line's point A -> " << borderLinePointARef->x << ", " << borderLinePointARef->y << ", " << borderLinePointARef->z << std::endl;
		direction = CyclingDirection::REVERSE;
	}
	else if (normalizedEmptyNormalY == normalizedBorderLinePointBY)
	{
		std::cout << "#- Cycling Direction    > Direction is FORWARD, going towards border line's point B -> " << borderLinePointBRef->x << ", " << borderLinePointBRef->y << ", " << borderLinePointBRef->z << std::endl;
		direction = CyclingDirection::FORWARD;
	}

	return direction;
	//pointARef = rotationpointsRefVector->
}

void QuatRotationManager::initializeAndRunforAligningNeighboringCleaveSequencesToPosY(QuatRotationPoints* in_quatpointsRefVector)
{
	rotationpointsRefVector = in_quatpointsRefVector;
	glm::vec3 pointToRotateTo = glm::normalize(rotationpointsRefVector->getLastPoint());
	rotateAroundZForPosYNormalAndPushIntoStack(pointToRotateTo);
}

void QuatRotationManager::executeRotationsForEmptyNormal()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//std::cout << "!! Rotation around Y required, performing... " << std::endl;
			rotateAroundYAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//std::cout << "!! Rotation around Z required, performing..." << std::endl;
			rotateAroundZAndPushIntoStack();
		}
	}

	if (pointCRef->y != pointARef->y)		// only rotate around X if point A's y and point C's y aren't equal (otherwise, its where we want it to be, which is all points having same Y)
	{
		//std::cout << "!! Rotation around X required, performing..." << std::endl;
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

	// only rotate around X if point C's z is not the same as point A's z.... otherwise, its where we want it to be, which is all points having same Z --AND-- pointCRef veers towards positive 1
	if 
	(
		(pointCRef->z != pointARef->z)		// must be on same Z coordinate
		//&&									// --AND--
		//(pointCRef->y > 0)					// y for the third point must be positive
	)
	{
		if (debugFlag == 1)
		{
			std::cout << "!!!! Rotation around X required, performing...(Z-fracture)" << std::endl;
		}
		rotateAroundXForZFractureAndPushIntoStack();
	}
}

void QuatRotationManager::executeRotationsForPlanarSlide()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Y required, performing... (Planar sliding)" << std::endl;
			//}
			rotateAroundYToPosZForPlanarSlideAndPushIntoStack();
		}
		else if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//if (debugFlag == 1)
			//{
				//std::cout << "!!!! Rotation around Z required, performing...(Planar sliding)" << std::endl;
			//}
			//rotateAroundZAndPushIntoStack();
			//rotateAroundXToYZeroAndPushIntoStack();
			rotateAroundXToYZeroForPlanarSlideAndPushIntoStack();
		}
	}

	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::executeRotationsForCyclingDirectionFinder()
{
	if (pointBRef->y != 0)
	{
		std::cout << "####--> Rotating around z..." << std::endl;
		rotateAroundZAndPushIntoStack();
	}
}

void QuatRotationManager::calculateEmptyNormal()
{
	glm::vec3 currentPoint0 = *rotationpointsRefVector->getFirstPointRef();
	glm::vec3 currentPoint1 = *rotationpointsRefVector->getSecondPointRef();
	glm::vec3 currentPoint2 = *rotationpointsRefVector->getThirdPointRef();
	glm::vec3 currentMRP = *rotationpointsRefVector->getMRPRef();

	glm::vec3* currentNormalRef = rotationpointsRefVector->getNormalRef();
	*currentNormalRef = cross(currentPoint1, currentPoint2);
	/*
	std::cout << "##!~~~~~ Points are: " << std::endl;
	std::cout << "0: " << currentPoint0.x << ", " << currentPoint0.y << ", " << currentPoint0.z << std::endl;
	std::cout << "1: " << currentPoint1.x << ", " << currentPoint1.y << ", " << currentPoint1.z << std::endl;
	std::cout << "2: " << currentPoint2.x << ", " << currentPoint2.y << ", " << currentPoint2.z << std::endl;
	std::cout << "MRP: " << currentMRP.x << ", " << currentMRP.y << ", " << currentMRP.z << std::endl;
	std::cout << "##!~~~~~ current normal is: " << currentNormalRef->x << ", " << currentNormalRef->y << ", " << currentNormalRef->z << std::endl;
	*/
	glm::vec3 calibratedEmptyNormal = checkForEmptyNormalCorrection(currentMRP, *currentNormalRef);
	*currentNormalRef = calibratedEmptyNormal;

	//std::cout << "##!~~~~~ (calibrated) current normal is: " << currentNormalRef->x << ", " << currentNormalRef->y << ", " << currentNormalRef->z << std::endl;

	//int someVal = 3;
	//std::cin >> someVal;
	//rotationpointsRefVector->pointsRefVector.push_back(currentNormal);
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
	glm::vec3* currentNormalRef = rotationpointsRefVector->getNormalRef();
	glm::vec3 returningNormal = *currentNormalRef;
	return returningNormal;
}

void QuatRotationManager::rotateAroundYAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
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
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);						// push into the stack

	//std::cout << "Printing points after Y-axis bound rotation: " << std::endl;
	//rotationpointsRefVector->printPoints();
	//return firstPassRotateRadians;
}

void QuatRotationManager::rotateAroundYToPosZForPlanarSlideAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
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

	//std::cout << "################### (Planar slide) Printing points after Y-axis rotation to pos Z: " << std::endl;
}

void QuatRotationManager::rotateAroundXToYZeroForPlanarSlideAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	float atan2result = atan2(triangleNormalRef->y, triangleNormalRef->z);

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
	//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;

	glm::vec3 rotationAroundY;
	rotationAroundY.x = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//std::cout << "Printing points after X-axis bound rotation: (the final rotation?) " << std::endl;
	//rotationpointsRefVector->printPoints();

}

void QuatRotationManager::rotateAroundZAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

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

	if (debugFlag == 1)
	{
		std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
	}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//std::cout << "Printing points after Z-axis bound rotation: " << std::endl;
	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::rotateAroundZToFindBorderLineEmptyNormalAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

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

	if (debugFlag == 1)
	{
		std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
	}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

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
	//std::cout << "Printing points after Z-axis bound rotation: " << std::endl;
	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::rotateAroundZToFindCoplanarCategorizedLineEmptyNormalAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

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

	if (debugFlag == 1)
	{
		std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
	}
	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	// check if the 3rd point (the centroid of the triangle is positive Y or negative Y; it's Y should never be 0, if we did things correctly (the triangle would be invalid).
	glm::vec3 determinedEmptyNormal;
	glm::vec3 currentCentroid = rotationpointsRefVector->getPointByIndex(2);
	if (currentCentroid.y > 0)
	{
		determinedEmptyNormal.y = -1;
	}
	else if (currentCentroid.y < 0)
	{
		determinedEmptyNormal.y = 1;
	}

	glm::vec3* emptyNormalRef = rotationpointsRefVector->getPointRefByIndex(3);
	*emptyNormalRef = determinedEmptyNormal;
}



void QuatRotationManager::rotateAroundZAndPushIntoStack(glm::vec3* in_point)
{
	if (in_point->y != 1.0f)
	{
		float radians = 0.0f;
		float fullRadian360 = 6.28319;

		//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
		//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
		float atan2result = atan2(in_point->y, in_point->x); // find the radians we'll need to rotate by
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

		if (debugFlag == 1)
		{
			std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
		}
		glm::vec3 rotationAroundZ;
		rotationAroundZ.z = -1.0f;
		QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

		glm::quat originalQuat = s1record.returnOriginalRotation();
		//*pointBRef = originalQuat * *pointBRef;	
		rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
		rotationRecords.push(s1record);
	}
}

void QuatRotationManager::rotateAroundXToYZeroAndPushIntoStack()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	float atan2result = atan2(pointCRef->y, pointCRef->z);

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
	//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;

	glm::vec3 rotationAroundY;
	rotationAroundY.x = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//std::cout << "Printing points after X-axis bound rotation: (the final rotation?) " << std::endl;
	//rotationpointsRefVector->printPoints();

}

void QuatRotationManager::rotateAroundXForZFractureAndPushIntoStack()
{
	glm::vec3 currentThirdPoint = *rotationpointsRefVector->getThirdPointRef();	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForZFracture(currentThirdPoint);	// get the number of radians to rotate by

	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);	// rotate all values by this one
	rotationRecords.push(s1record);

	//rotationpointsRefVector->printPoints();
}

void QuatRotationManager::rotateAroundXForPositiveYBorderLineAndPushIntoStack()
{
	glm::vec3 currentNormalValue = *rotationpointsRefVector->getPointRefByIndex(3);	// get a copy of the value of the 3rd primal point
	float radiansToRotateBy = findRotationRadiansForZFracture(currentNormalValue);	// get the number of radians to rotate by
	std::cout << "::> radiansToRotateBy " << radiansToRotateBy << std::endl;

	//std::cout << "!!! Points in poly plane will be rotated by this many radians to get to Pos Y: " << radiansToRotateBy << std::endl;
	glm::vec3 rotateAroundX;
	rotateAroundX.x = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);	// rotate all values by this one
	rotationRecords.push(s1record);
}

void QuatRotationManager::rotateEmptyNormalToPosY(glm::vec3* in_normal)
{
	if (in_normal->x != 1.0f)	// rotate around y first, to x = 1.0f
	{
		float radiansToRotateBy = findRotationRadainsForGettingToPosXThroughY(*in_normal);

		glm::vec3 rotateAroundY;
		rotateAroundY.y = 1.0f;
		QuatRotationRecord s1record(radiansToRotateBy, rotateAroundY);

		glm::quat fractureQuat = s1record.returnOriginalRotation();
		rotationpointsRefVector->applyQuaternion(fractureQuat);
		rotationRecords.push(s1record);
	}

	// rotate to pos Y if it wasn't set already (y = 1.0f)
	if (in_normal->y != 1.0f)
	{
		rotateAroundZForPosYNormalAndPushIntoStack(*in_normal);
	}

}

void QuatRotationManager::rotateAroundZForPosYNormalAndPushIntoStack(glm::vec3 in_normal)
{
	float radiansToRotateBy = findRotationRadiansForGettingToPosYThroughZ(in_normal);

	glm::vec3 rotateAroundZ;
	rotateAroundZ.z = -1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundZ);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);
	rotationRecords.push(s1record);
}

float QuatRotationManager::findRotationRadiansForZFracture(glm::vec3 in_vec3)
{

	// The overarching goal is to get to POS Y for this 3rd point (3rd point is the value that was passed in)
	std::cout << ">>>>> vec3 value is: " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;

	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians = 0.0f;


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
	}
	degreesToRotateOnX = secondPassRotateFinal;
	return degreesToRotateOnX;
}

float QuatRotationManager::findRotationRadainsForGettingToPosXThroughY(glm::vec3 in_vec3)
{
	float degreesToRotateOnY = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.z, in_vec3.x);
	float firstPassRotateRadians = 0.0f;

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-X, via Y-axis) Passed point is:  " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-X, via Y-axis) Rotating passed point to Pos X (X = 1.0f), and finding degrees to rotate to get to it... " << std::endl;

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
	rotationAngle.y = 1.0f;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosX;
	targetPosX.x = 1.0f;
	targetPosX = rotationQuat * targetPosX;


	//std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	//std::cout << "Target pos X is at: " << targetPosX.x << ", " << targetPosX.y << ", " << targetPosX.z << std::endl;
	//std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;
	//std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosX.z, targetPosX.x);

	if (debugFlag == 1)
	{
		std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	}
	float secondPassRotateRadians = 0.0f;
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

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-X, via Y-axis) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-X, via Y-axis) It was rotated to pos X, via this many radians: " << secondPassRotateFinal << std::endl;
	//std::cout << std::endl;

	degreesToRotateOnY = secondPassRotateFinal;

	//int someVal = 3;
	//std::cin >> someVal;


	return degreesToRotateOnY;
}


float QuatRotationManager::findRotationRadiansForGettingToPosYThroughZ(glm::vec3 in_vec3)
{
	// The overarching goal is to get to POS Y for this 3rd point(3rd point is the value that was passed in), by rotating around the Z axis

	float degreesToRotateOnZ = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.x);
	float firstPassRotateRadians = 0.0f;

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Passed point is:  " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Rotating passed point to Pos X (X = 1.0f), and finding degrees to rotate to get to it... " << std::endl;


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

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;
	//std::cout << std::endl;

	degreesToRotateOnZ = secondPassRotateFinal;

	//int someVal = 3;
	//std::cin >> someVal;


	return degreesToRotateOnZ;
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
		//std::cout << "################# Reversing values... " << std::endl;
		glm::quat reverseQuat = currentRecord.returnReverseRotation();
		rotationpointsRefVector->applyQuaternion(reverseQuat);
		//rotationpointsRefVector->printPoints();

		rotationRecords.pop();
	}
}

void QuatRotationManager::setDebugFlag(int in_debugFlag)
{
	debugFlag = in_debugFlag;
}