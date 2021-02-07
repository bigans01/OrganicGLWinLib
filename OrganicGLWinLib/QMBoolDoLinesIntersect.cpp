#include "stdafx.h"
#include "QMBoolDoLinesIntersect.h"

bool QMBoolDoLinesIntersect::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// Remember: this function presumes we are in the XY plane -- the Z coordinate should be 0.

	// Point 0 = first line, point A
	// Point 1 = first line, point B
	// Point 2 = second line, point A
	// point 3 = second line, point B
	// point 4 = resultant intersecting point, if it is found.

	bool returnValue = false;

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

	// now that we have rotated the first line, such that it's X > 0, Y ~= 0, and Z ~= 0, we will check whether or not the second line intersects.
	bool intersectDetected = false;

	std::cout << "***** First Line point A: " << in_quatRotationPointsRef->getPointRefByIndex(0)->x << ", " << in_quatRotationPointsRef->getPointRefByIndex(0)->y << std::endl;
	std::cout << "***** First Line point B: " << in_quatRotationPointsRef->getPointRefByIndex(1)->x << ", " << in_quatRotationPointsRef->getPointRefByIndex(1)->y << std::endl;

	// first, check if the points of the second line are such that one is in positive y, and the other is in negative y.
	glm::vec3* secondLinePointARef = in_quatRotationPointsRef->getPointRefByIndex(2);
	glm::vec3* secondLinePointBRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	
	std::cout << "***** Second Line point A: " << secondLinePointARef->x << ", " << secondLinePointARef->y << std::endl;
	std::cout << "***** Second Line point B: " << secondLinePointBRef->x << ", " << secondLinePointBRef->y << std::endl;


	// remember, points A and B of the first line have y = 0, after the call to rotateLineToYZeroPositiveX.
	if
	(
		(
			(secondLinePointARef->y >= 0)
			&&
			(secondLinePointBRef->y <= 0)
		)

		||

		(
			(secondLinePointARef->y <= 0)
			&&
			(secondLinePointBRef->y >= 0)
		)
	)
	{
		std::cout << "************ entered first comparison branch.  " << std::endl;
		// get the length of the line
		float lengthOfInterceptingLine = glm::distance(*secondLinePointARef, *secondLinePointBRef);
		glm::vec3 resultantSlope = *secondLinePointBRef - *secondLinePointARef;
		float distance_to_travel_to_y_0 = 0.0f;
		float y_diff;
		glm::vec3 calculatedPoint;

		// first check: is point B of the second line exactly on 0?
		if (secondLinePointBRef->y == 0.0f)
		{
			std::cout << "**** notice: point B is exactly on 0. " << std::endl;
			distance_to_travel_to_y_0 = 1;
		}

		// second check: is point A of the second line exactly on 0?
		else if (secondLinePointARef->y == 0.0f)
		{
			std::cout << "**** notice: point A is exactly on 0; no travelling to get to y = 0. " << std::endl;
		}

		// other wise, check normally.
		else if (secondLinePointBRef->y > 0)
		{
			std::cout << "**** notice: point B >= 0. " << std::endl;
			y_diff = abs(0 - secondLinePointARef->y);	// get the distance that point A has to travel, to get to Y = 0; must subtract since A is < 0.
			distance_to_travel_to_y_0 = y_diff / resultantSlope.y;
		}
		else if (secondLinePointBRef->y < 0)
		{
			std::cout << "**** notice: point A >= 0. " << std::endl;
			y_diff = abs(secondLinePointARef->y);	// get the distance that point A has to travel, to get to Y = 0; value is equal to y of point A.
			distance_to_travel_to_y_0 = y_diff / resultantSlope.y;
		}


		
		calculatedPoint.x = secondLinePointARef->x + (resultantSlope.x * distance_to_travel_to_y_0);
		calculatedPoint.y = secondLinePointARef->y + (resultantSlope.y * distance_to_travel_to_y_0);

		// ensure that the resulting calculated point has an X <= to the second point of the first line (because that point has X >= 0)
		if (calculatedPoint.x <= in_quatRotationPointsRef->getPointByIndex(1).x)
		{
			std::cout << "!!! Valid intersection point found. " << std::endl;
			std::cout << "!!! Point is: " << calculatedPoint.x << ", " << calculatedPoint.y << std::endl;

			glm::vec3* intersectedPointRef = in_quatRotationPointsRef->getPointRefByIndex(4);
			*intersectedPointRef = calculatedPoint;
			rotatePointsToOriginalPosition(&rotationRecords, in_quatRotationPointsRef);
			returnValue = true;
		}

	}
	else
	{

	}

	return returnValue;
}

void QMBoolDoLinesIntersect::rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef)
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