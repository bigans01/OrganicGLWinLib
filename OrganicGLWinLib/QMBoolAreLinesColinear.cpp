#include "stdafx.h"
#include "QMBoolAreLinesColinear.h"

bool QMBoolAreLinesColinear::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// Remember: this function presumes we are in the XY plane -- the Z coordinate should be 0.

	// Point 0 = first line, point A
	// Point 1 = first line, point B
	// Point 2 = second line, point A
	// point 3 = second line, point B

	bool areLinesColinear = false;

	// first, check for point translation; do this by checking point A in the first line.
	glm::vec3 pointACopy = in_quatRotationPointsRef->getPointByIndex(0);
	PointTranslationCheck pointCheck;
	pointCheck.performCheck(pointACopy);
	if (pointCheck.requiresTranslation == 1)
	{
		in_quatRotationPointsRef->applyTranslation(pointCheck.getTranslationValue());
	}

	// check if the second point of the first line, is aligned such that x > 0, and y = 0.
	// if neither of these are true, we must rotate along the Z axis to positive X.
	std::vector<QuatRotationType> rotationOrder;
	std::stack<QuatRotationRecord> rotationRecords;
	glm::vec3* lineAPointBRef = in_quatRotationPointsRef->getPointRefByIndex(1);
	if
	(
		(lineAPointBRef->x <= 0.0f)
		||
		(lineAPointBRef->y != 0.0f)
	)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Z;
		rotationOrder.push_back(rotateType);		// should call rotateAroundZToYZero();
	}
	rotateLineToYZeroPositiveX(lineAPointBRef, &rotationRecords, in_quatRotationPointsRef, &rotationOrder);

	// now that all lines are aligned to be Y = 0, and X > 0, insert a new point that is just Y = 1. 
	glm::vec3 upwardNormal;
	upwardNormal.y = 1.0f;
	in_quatRotationPointsRef->insertPointRefs(&upwardNormal);

	// next, rotate the normal around Z to Y zero.
	rotateAroundZToYZero(&upwardNormal, in_quatRotationPointsRef, &rotationRecords);

	// perform two right angle checks: upwardNormal vs second line's pointA, and upwardNormal vs second line's pointB
	//std::cout << "||||||:::: BEGIN: print points, before coplanarity test: " << std::endl;
	//in_quatRotationPointsRef->printPoints();

	//std::cout << "||||||:::: BEGIN: Coplanarity test, running right-angle checks: " << std::endl;
	bool checkPointA = runCoplanarCheck(upwardNormal, in_quatRotationPointsRef->getPointByIndex(2));	// point A of second line
	bool checkPointB = runCoplanarCheck(upwardNormal, in_quatRotationPointsRef->getPointByIndex(3));	// point B of second line
	//std::cout << "||||||:::: END: Coplanarity test, running right-angle checks: " << std::endl;
	//int waitVal = 3;
	//std::cin >> waitVal;

	if
	(
		(checkPointA == true)
		&&
		(checkPointB == true)
	)
	{
		areLinesColinear = true;
		//std::cout << "!!! Both points found as colinear; returning TRUE." << std::endl;
	}
	else
	{
		//std::cout << "!!! Both points ARE NOT colinear; returning FALSE. " << std::endl;
	}

	return areLinesColinear;
}

void QMBoolAreLinesColinear::rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	auto vectorBegin = (*in_rotationOrderVectorRef).begin();
	auto vectorEnd = (*in_rotationOrderVectorRef).end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			//std::cout << "!!! Rotate around Z detected." << std::endl;

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

			//std::cout << "!!! Printing resulting points. " << std::endl;
			//in_quatRotationPointsRef->printPoints();
		}
	}
}

bool QMBoolAreLinesColinear::runCoplanarCheck(glm::vec3 in_upwardNormalRef, glm::vec3 in_pointToCompareTo)
{
	bool wasRightAngleFound = false;
	float radians = 0.0f;
	float fullRadian360 = 6.28319;

	//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
	//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
	float atan2result = atan2(in_pointToCompareTo.y, in_pointToCompareTo.x); // find the radians we'll need to rotate by
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
	//quatRotationManagerLogger.log(">>>> Check for right angle radians (pre-round) is: ", firstPassRotateRadians, "\n");
	firstPassRotateRadians = roundRadiansForRightAngleCheck(firstPassRotateRadians);
	std::cout << ">>>> Check for right angle radians (post-round) is: " << firstPassRotateRadians << std::endl;
	//quatRotationManagerLogger.log(">>>> Check for right angle radians (post-round) is: ", firstPassRotateRadians, "\n");
	if 
	(	
		(firstPassRotateRadians == 1.5708f) // 90 degrees
		||
		(firstPassRotateRadians == (1.5708f * 3)) // 270 degrees
	)
	{
		wasRightAngleFound = true;
		//std::cout << "!!! Right angle detected. " << std::endl;
	}

	return wasRightAngleFound;
}

float QMBoolAreLinesColinear::roundRadiansForRightAngleCheck(float in_angleInRadians)
{
	float calcedFloat = float(floor(in_angleInRadians * 10000 + 0.5) / 10000);
	return calcedFloat;
}

void QMBoolAreLinesColinear::rotateAroundZToYZero(glm::vec3* in_upwardNormalRef, QuatRotationPoints* in_quatRotationPointsRef, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef)
{
	if (in_upwardNormalRef->y != 0.0f)
	{
		float radians = 0.0f;
		float fullRadian360 = 6.28319;

		//std::cout << "!! Point B x is: " << pointBRef->x << std::endl;
		//std::cout << "!! Point B y is: " << pointBRef->y << std::endl;
		float atan2result = atan2(in_upwardNormalRef->y, in_upwardNormalRef->x); // find the radians we'll need to rotate by
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

		//if (debugFlag == 1)
		//{
			//std::cout << "First pass rotate radians is: " << firstPassRotateRadians << std::endl;
		//}
		glm::vec3 rotationAroundZ;
		rotationAroundZ.z = 1.0f;
		QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

		glm::quat originalQuat = s1record.returnOriginalRotation();
		//*pointBRef = originalQuat * *pointBRef;	
		in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
		in_quatRotationRecordStackRef->push(s1record);

		//std::cout << ":::: Observer calculation: Printing points after Z-axis bound rotation: " << std::endl;
		//rotationpointsRefVector->printPoints();

		//std::cout << ":::: Radian value is: " << radianValue << std::endl;
	}
}