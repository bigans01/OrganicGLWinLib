#include "stdafx.h"
#include "QMBoolDoLinesIntersect.h"

bool QMBoolDoLinesIntersect::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// set logging level
	qmBoolBaseLoggerDebugLevel = in_polyDebugLevel;
	qmBoolBaseLogger.setDebugLevel(qmBoolBaseLoggerDebugLevel);

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

	//std::cout << "|||||||||||| BEGIN: (do lines intersect?) printing points " << std::endl;
	//in_quatRotationPointsRef->printPoints();
	//std::cout << "|||||||||||| END: (do lines intersect?) printing points " << std::endl;

	// now that we have rotated the first line, such that it's X > 0, Y ~= 0, and Z ~= 0, we will check whether or not the second line intersects.
	bool intersectDetected = false;

	//std::cout << "***** First Line point A: " << in_quatRotationPointsRef->getPointRefByIndex(0)->x << ", " << in_quatRotationPointsRef->getPointRefByIndex(0)->y << std::endl;
	//std::cout << "***** First Line point B: " << in_quatRotationPointsRef->getPointRefByIndex(1)->x << ", " << in_quatRotationPointsRef->getPointRefByIndex(1)->y << std::endl;

	// first, check if the points of the second line are such that one is in positive y, and the other is in negative y.
	glm::vec3* secondLinePointARef = in_quatRotationPointsRef->getPointRefByIndex(2);
	glm::vec3* secondLinePointBRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	
	//std::cout << "***** Second Line point A: " << secondLinePointARef->x << ", " << secondLinePointARef->y << std::endl;
	//std::cout << "***** Second Line point B: " << secondLinePointBRef->x << ", " << secondLinePointBRef->y << std::endl;

	// round the Y by 10-thousandths.
	roundVec3YByTenThousandths(secondLinePointARef);
	roundVec3YByTenThousandths(secondLinePointBRef);

	//std::cout << "::: secondLinePointARef->y = " << secondLinePointARef->y << std::endl;
	//std::cout << "::: secondLinePointBRef->y = " << secondLinePointBRef->y << std::endl;
	qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): ::: secondLinePointARef->y = ", secondLinePointARef->y, "\n");
	qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): ::: secondLinePointBRef->y = ", secondLinePointBRef->y, "\n");


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
		//std::cout << "************ entered first comparison branch.  " << std::endl;
		qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): ************ entered first comparison branch.  ", "\n");
		// get the length of the line
		float lengthOfInterceptingLine = glm::distance(*secondLinePointARef, *secondLinePointBRef);
		glm::vec3 resultantSlope = *secondLinePointBRef - *secondLinePointARef;
		float distance_to_travel_to_y_0 = 0.0f;
		float a_absolute;
		glm::vec3 calculatedPoint;


		// first check: is point B of the second line exactly on 0?
		if (secondLinePointBRef->y == 0.0f)
		{
			//std::cout << "**** notice: point B is exactly on 0. " << std::endl;
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): **** notice: point B is exactly on 0. ", "\n");
			distance_to_travel_to_y_0 = 1;
		}

		// second check: is point A of the second line exactly on 0?
		else if (secondLinePointARef->y == 0.0f)
		{
			//std::cout << "**** notice: point A is exactly on 0; no travelling to get to y = 0. " << std::endl;
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): **** notice: point A is exactly on 0; no travelling to get to y = 0. ", "\n");
		}

		// other wise, check normally.
		else
		{
			//std::cout << "**** entered third branch (normal)" << std::endl;
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): **** entered third branch (normal)", "\n");
			// because the cuttable line is such that Y = 0 and X > 0, 
			// and the cutting line may or may not intersect somehow (not determined at this point), it holds true that the intersecting point would be at Y = 0.
			// The distance to travel to get to Y can then be calculated by getting the absolute value of the cutting line's point A, 
			// divided by the combined absolute values of the of Y-values for each point. The resulting value is the
			// percentage of the distance of the line, to get to Y = 0. Multiplying the slope of the cutting line by this percentage, 
			// results in the intersecting point.

			//float ab_diff = abs(secondLinePointBRef->y - secondLinePointARef->y); // the total y-difference between the two points
			float ab_combinedAbsolute = abs(secondLinePointBRef->y) + abs(secondLinePointARef->y); // the total y-difference between the two points
			a_absolute = abs(secondLinePointARef->y);
			distance_to_travel_to_y_0 = a_absolute / ab_combinedAbsolute;

			//std::cout << ":::: distance to y is: " << distance_to_travel_to_y_0 << std::endl;
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): :::: distance to y is: ", distance_to_travel_to_y_0, "\n");
		}

		//std::cout << "+++++++++ printing points: " << std::endl;
		//in_quatRotationPointsRef->printPoints();


		//std::cout << ":: secondLine, pointA, before applying slope: " << secondLinePointARef->x << ", " << secondLinePointARef->y << std::endl;
		//std::cout << ":: secondLine, pointB, before applying slope: " << secondLinePointBRef->x << ", " << secondLinePointBRef->y << std::endl;
		//std::cout << ":: slope: " << resultantSlope.x << ", " << resultantSlope.y << std::endl;
		qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): :: secondLine, pointA, before applying slope: ", secondLinePointARef->x, ", ", secondLinePointARef->y, "\n");
		qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): :: secondLine, pointB, before applying slope: ", secondLinePointBRef->x, ", ", secondLinePointBRef->y, "\n");
		qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): :: slope: ", resultantSlope.x, ", ", resultantSlope.y, "\n");

		//std::cout << "----> distance_to_travel_to_y_0 = " << distance_to_travel_to_y_0 << std::endl;
		calculatedPoint.x = secondLinePointARef->x + (resultantSlope.x * distance_to_travel_to_y_0);
		calculatedPoint.y = secondLinePointARef->y + (resultantSlope.y * distance_to_travel_to_y_0);

		//std::cout << "----> calculated point, before rotation back: " << calculatedPoint.x << ", " << calculatedPoint.y  
			//      << "||| first line, point A: " << in_quatRotationPointsRef->getPointByIndex(0).x << ", " << in_quatRotationPointsRef->getPointByIndex(0).y
				//  << "||| point B: " << in_quatRotationPointsRef->getPointByIndex(1).x << ", " << in_quatRotationPointsRef->getPointByIndex(1).y << std::endl;
		qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): ----> calculated point, before rotation back: ", calculatedPoint.x, ", ", calculatedPoint.y,
						" ||| first line, point A: ", in_quatRotationPointsRef->getPointByIndex(0).x, ", ", in_quatRotationPointsRef->getPointByIndex(0).y,
						" ||| point B: ", in_quatRotationPointsRef->getPointByIndex(1).x, ", ", in_quatRotationPointsRef->getPointByIndex(1).y, "\n");


		// ensure that the resulting calculated point has an X <= to the second point of the first line (because that point has X >= 0)
		// --AND--
		// that it's X is greater than 0 (because point A of the first line starts at X = 0, if it is less than 0 it doesn't intersect.)
		if 
		(
			(calculatedPoint.x <= in_quatRotationPointsRef->getPointByIndex(1).x)
			&&
			(calculatedPoint.x >= 0)
		)
		{
			//std::cout << "!!! Valid intersection point found. " << std::endl;
			//std::cout << "!!! Point is: " << calculatedPoint.x << ", " << calculatedPoint.y << std::endl;
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): !!! Valid intersection point found. ", "\n");
			qmBoolBaseLogger.log("(QMBoolDoLinesIntersect): !!! Point is: ", calculatedPoint.x, ", ", calculatedPoint.y, "\n");

			glm::vec3* intersectedPointRef = in_quatRotationPointsRef->getPointRefByIndex(4);
			*intersectedPointRef = calculatedPoint;
			rotatePointsToOriginalPosition(&rotationRecords, in_quatRotationPointsRef);
			if (pointCheck.requiresTranslation == 1)
			{
				in_quatRotationPointsRef->applyTranslation(pointCheck.getReverseTranslationValue());
			}
			returnValue = true;
		}


		//std::cout << "+++++++++ printing points: " << std::endl;
		//in_quatRotationPointsRef->printPoints();

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

void QMBoolDoLinesIntersect::roundVec3YByTenThousandths(glm::vec3* in_vec3Ref)
{
	in_vec3Ref->y = float(floor(in_vec3Ref->y * 10000 + 0.5) / 10000);
}