#include "stdafx.h"
#include "QMBoolIsColinearLineAContainedWithinB.h"

bool QMBoolIsColinearLineAContainedWithinB::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
		// Assumption #1: points are in the XY plane -- the Z coordinate should be 0.
		// Assumption #2: line was determined as being colinear (should be properly verified by QMBoolAreLinesColinear)

		// Point 0 = first line, point A
		// Point 1 = first line, point B
		// Point 2 = second line, point A
		// point 3 = second line, point B

		bool containedWithinB = false;

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

		// next, round all 4 points to the nearest ten-thousandth.
		for (int x = 0; x < 4; x++)
		{
			roundVec3YByTenThousandths(in_quatRotationPointsRef->getPointRefByIndex(x));
		}

		// check for containment, in points A and B of line A.
		bool isLineAPointAContainedWithinLineB = checkIfLineAPointIsContainedWithinLineB(in_quatRotationPointsRef->getPointByIndex(0), in_quatRotationPointsRef);
		bool isLineAPointBContainedWithinLineB = checkIfLineAPointIsContainedWithinLineB(in_quatRotationPointsRef->getPointByIndex(1), in_quatRotationPointsRef);
		if
		(
			(isLineAPointAContainedWithinLineB == true)
			&&
			(isLineAPointBContainedWithinLineB == true)
		)
		{
			containedWithinB = true;
		}
		return containedWithinB;
}

void QMBoolIsColinearLineAContainedWithinB::rotateLineToYZeroPositiveX
(
	glm::vec3* in_pointToRotateFor, 
	std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, 
	QuatRotationPoints* in_quatRotationPointsRef, 
	std::vector<QuatRotationType>* in_rotationOrderVectorRef
)
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

void QMBoolIsColinearLineAContainedWithinB::roundVec3YByTenThousandths(glm::vec3* in_vec3Ref)
{
	in_vec3Ref->y = float(floor(in_vec3Ref->y * 10000 + 0.5) / 10000);
}

bool QMBoolIsColinearLineAContainedWithinB::checkIfLineAPointIsContainedWithinLineB(glm::vec3 in_pointToCheck, QuatRotationPoints* in_quatRotationPointsRef)
{
	bool isContained = false;
	glm::vec3* lineBPointARef = in_quatRotationPointsRef->getPointRefByIndex(2);
	glm::vec3* lineBPointBRef = in_quatRotationPointsRef->getPointRefByIndex(3);

	// to be considered "contained", the pointToCheck must have it's x >= lineBPointARef->x, and <= lineBPointBRef->x.
	if
	(
		(in_pointToCheck.x >= lineBPointARef->x)
		&&
		(in_pointToCheck.x <= lineBPointBRef->x)
	)
	{
		isContained = true;
	}
	return isContained;
}