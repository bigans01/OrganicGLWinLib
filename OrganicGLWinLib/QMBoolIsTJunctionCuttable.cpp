#include "stdafx.h"
#include "QMBoolIsTJunctionCuttable.h"

bool QMBoolIsTJunctionCuttable::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	bool isCuttable = false;
	// Remember: this function presumes we are in the XY plane -- the Z coordinate should be 0.

	// Point 0 = line point A
	// Point 1 = line point B
	// Point 2 = pointToCheck
	// point 3 = normal of line A

	// first, check for point translation; do this by checking point A in the first line.
	glm::vec3 pointACopy = in_quatRotationPointsRef->getPointByIndex(0);
	PointTranslationCheck pointCheck;
	pointCheck.performCheck(pointACopy);
	if (pointCheck.requiresTranslation == 1)
	{
		// apply the translation, only to the first 3 points.
		in_quatRotationPointsRef->applyTranslationToIndexRange(pointCheck.getTranslationValue(), 0, 2);
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
	glm::vec3* normalRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	glm::vec3* pointToCheckRef = in_quatRotationPointsRef->getPointRefByIndex(2);
	if 
	(
		(normalRef->y > 0)
		&&
		(pointToCheckRef->y > 0)
	)
	{
		isCuttable = true;
	}
	else if 
	(
		(normalRef->y < 0)
		&&
		(pointToCheckRef->y < 0)
	)
	{
		isCuttable = true;
	}

	return isCuttable;
}

void QMBoolIsTJunctionCuttable::rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
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