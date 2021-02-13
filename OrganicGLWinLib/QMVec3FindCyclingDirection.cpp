#include "stdafx.h"
#include "QMVec3FindCyclingDirectionPoint.h"

glm::vec3 QMVec3FindCyclingDirectionPoint::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	glm::vec3 returnVec;
	// Remember: this function presumes we are in the XY plane -- the Z coordinate should be 0.

	// Point 0 = first line, point A
	// Point 1 = first line, point B
	// Point 2 = second line, point A
	// point 3 = second line, point B
	// point 4 = the normal to use to determine whether or not we'll use point A or point B of the second line.

	// first, store the original values of the points we're analyzing.
	originalCandidatePointA = in_quatRotationPointsRef->getPointByIndex(2);
	originalCandidatePointB = in_quatRotationPointsRef->getPointByIndex(3);

	// second, check for point translation; do this by checking point A in the first line.
	glm::vec3 pointACopy = in_quatRotationPointsRef->getPointByIndex(0);
	PointTranslationCheck pointCheck;
	pointCheck.performCheck(pointACopy);
	if (pointCheck.requiresTranslation == 1)
	{
		// apply the translation, only to the first 4 points.
		in_quatRotationPointsRef->applyTranslationToIndexRange(pointCheck.getTranslationValue(), 0, 3);
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

	// ensure that the normal we're using is on positive Y; flip it to positive Y if not.
	glm::vec3* normalRef = in_quatRotationPointsRef->getPointRefByIndex(4);
	if (normalRef->y < 0)
	{
		flipOnXAxis(in_quatRotationPointsRef);
	}


	// now, find out which point is on positive Y; the same binary Y value as the normal.
	if (in_quatRotationPointsRef->getPointByIndex(2).y > 0)
	{
		returnVec = originalCandidatePointA;
	}
	else if (in_quatRotationPointsRef->getPointByIndex(3).y > 0)
	{
		returnVec = originalCandidatePointB;

	}
	return returnVec;
}

void QMVec3FindCyclingDirectionPoint::rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
{
	auto vectorBegin = (*in_rotationOrderVectorRef).begin();
	auto vectorEnd = (*in_rotationOrderVectorRef).end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Z)
		{
			std::cout << "!!! Rotate around Z detected." << std::endl;

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

			std::cout << "!!! Printing resulting points. " << std::endl;
			in_quatRotationPointsRef->printPoints();
		}
	}
}
