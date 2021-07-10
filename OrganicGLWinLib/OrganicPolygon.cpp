#include "stdafx.h"
#include "OrganicPolygon.h"

void OrganicPolygon::addPoint(ECBPolyPoint in_ECBPolyPoint)
{
	points[numberOfPoints++] = in_ECBPolyPoint;	// add the ECBPolyPoint to the array
}

void OrganicPolygon::calculateCenteredPoint()
{
	// this is called after all points have been added.
	int numberOfTriangles = numberOfPoints - 2;
	if (debugFlag == 1)
	{
		std::cout << "Number of triangles in this secondary: " << numberOfTriangles << std::endl;
	}
	if (numberOfTriangles == 1)
	{
		setCenteredPointViaTriangleCenter();
	}
	else if (numberOfTriangles >= 2)
	{
		int secondPoint = getValidEndpoint();
		setCenteredPointViaMidpoint(points[0], points[secondPoint]);
	}
}

int OrganicPolygon::getValidEndpoint()
{
	int returnIndex = 2;	// default is 2; would be returned if the very first line was valid.
	int point2index = 1;	// incremented by one every loop tick
	int point3index = 2;	// ""

	// get the number of lines to check
	int numberOfTriangles = numberOfPoints - 2;
	int numberOfMatches = 0;	// if this number is equal to 2, we'll have to break out of the loop

	for (int x = 0; x < numberOfTriangles; x++)
	{
		numberOfMatches = 0;	// reset the matches value
		ECBPolyPoint pointA = points[0];
		ECBPolyPoint pointB = points[point2index];
		ECBPolyPoint pointC = points[point3index];

		// check x
		if ((pointA.x == pointB.x) && (pointB.x == pointC.x))
		{
			numberOfMatches++;
		}

		// check y
		if ((pointA.y == pointB.y) && (pointB.y == pointC.y))
		{
			numberOfMatches++;
		}

		// check z
		if ((pointA.z == pointB.z) && (pointB.z == pointC.z))
		{
			numberOfMatches++;
		}

		if (numberOfMatches < 2)		// if the 3 points have a condition where 2 of the dimensions are exactly the same across all 3 points (i.e., x is 1 for all points, y is .5 for all points), we have an invalid triangle, and must ignore this line. 
										// Otherwise, select the line.
		{
			break;
		}
		point2index++;		// if the very first line (consisting of endpoints of point[0] and point[2]) is invalid, we didn't break; so we need to increment these values.
		point3index++;		// ""
		returnIndex++;		// iterate to the next endpoint for checking.
	}
	return returnIndex;
}

void OrganicPolygon::setCenteredPointViaTriangleCenter()
{
	if (debugFlag == 1)
	{
		std::cout << "!!! WARNING: singular triangle detected. " << std::endl;
	}

	// find midpoint between points 0 and 1


	ECBPolyPoint pointA = points[0];
	ECBPolyPoint pointB = points[1];
	ECBPolyPoint midpoint;
	//midpoint.x = (pointB.x - pointA.x) / 2;
	//midpoint.y = (pointB.y - pointA.y) / 2;
	//midpoint.z = (pointB.z - pointA.z) / 2;

	midpoint.x = pointA.x + ((pointB.x - pointA.x) / 2);
	midpoint.y = pointA.y + ((pointB.y - pointA.y) / 2);
	midpoint.z = pointA.z + ((pointB.z - pointA.z) / 2);

	ECBPolyPoint pointC = points[2];

	ECBPolyPoint centroidPointOffset;
	//centroidPointOffset.x = (pointC.x - midpoint.x) * (1 / 3);
	//centroidPointOffset.y = (pointC.y - midpoint.y) * (1 / 3);
	//centroidPointOffset.z = (pointC.z - midpoint.z) * (1 / 3);

	centroidPointOffset.x = midpoint.x + ((pointC.x - midpoint.x) * (1.0f / 3.0f));
	centroidPointOffset.y = midpoint.y + ((pointC.y - midpoint.y) * (1.0f / 3.0f));
	centroidPointOffset.z = midpoint.z + ((pointC.z - midpoint.z) * (1.0f / 3.0f));

	//centeredPoint.x = midpoint.x + centroidPointOffset.x;
	//centeredPoint.y = midpoint.y + centroidPointOffset.y;
	//centeredPoint.z = midpoint.z + centroidPointOffset.z;
	centeredPoint.x = centroidPointOffset.x;
	centeredPoint.y = centroidPointOffset.y;
	centeredPoint.z = centroidPointOffset.z;

	if (debugFlag == 1)
	{
		std::cout << ">> pointC x: " << pointC.x << std::endl;
		std::cout << ">> midpoint x: " << midpoint.x << std::endl;
		std::cout << ">> calc: " << ((pointC.x - midpoint.x) * (1 / 3)) << std::endl;
		std::cout << ">> 1/3: " << 1.0f / 3.0f << std::endl;
		std::cout << "Midpoint is: " << midpoint.x << ", " << midpoint.y << ", " << midpoint.z << ", " << std::endl;
		std::cout << "Centroid point offset is: " << centroidPointOffset.x << ", " << centroidPointOffset.y << ", " << centroidPointOffset.z << std::endl;
		std::cout << "Centered point will be at: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
	}

}

void OrganicPolygon::setCenteredPointViaMidpoint(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	ECBPolyPoint slope;
	slope.x = in_pointB.x - in_pointA.x;	// calculate base slope coords
	slope.y = in_pointB.y - in_pointA.y;
	slope.z = in_pointB.z - in_pointA.z;


	// divide the slope by 2 to get the midpoint
	slope.x /= 2;
	slope.y /= 2;
	slope.z /= 2;

	centeredPoint.x = in_pointA.x + slope.x;
	centeredPoint.y = in_pointA.y + slope.y;
	centeredPoint.z = in_pointA.z + slope.z;

	if (debugFlag == 1)
	{
		std::cout << "Centered point will be at: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
	}
}

void OrganicPolygon::findXAxisRotationPointB()
{
	int numberOfDefiningLines = numberOfPoints;	// 3 points = 3 lines (1 triangle); 5 points = 5 lines (3 triangles)
	int currentPointAIndex = 0;		// start with the very first point
	int currentPointBIndex = 1;
	for (int x = 0; x < numberOfDefiningLines; x++)
	{
		ECBPolyPoint currentPointA = points[currentPointAIndex];
		ECBPolyPoint currentPointB = points[currentPointBIndex];
		if
			(
			(
				(currentPointA.x >= centeredPoint.x)		// is the centered point y between the y of point A and B? 
				&&
				(currentPointB.x <= centeredPoint.x)
				)
				)
		{
			if (debugFlag == 1)
			{
				std::cout << "X-ROTATE: (Case 1) X-axis begin --> Valid line found." << std::endl;
				std::cout << "Point A index: " << currentPointAIndex << std::endl;
				std::cout << "Point B index: " << currentPointBIndex << std::endl;
			}
			findPointBForXAxisRotationTheta(currentPointA, currentPointB, centeredPoint);
			break;
		}
		else if
			(
			(
				(currentPointA.x <= centeredPoint.x)		// vice-versa
				&&
				(currentPointB.x >= centeredPoint.x)
				)
				)
		{
			if (debugFlag == 1)
			{
				std::cout << "X-ROTATE: (Case 2) X-axis begin --> Valid line found." << std::endl;
				std::cout << "Point A index: " << currentPointAIndex << std::endl;
				std::cout << "Point B index: " << currentPointBIndex << std::endl;
			}
			findPointBForXAxisRotationTheta(currentPointA, currentPointB, centeredPoint);
			break;
		}
		currentPointAIndex++;
		currentPointBIndex++;
	}
}

void OrganicPolygon::findYAxisRotationPointB()
{
	int numberOfDefiningLines = numberOfPoints;
	int currentPointAIndex = 0;		// start with the very first point
	int currentPointBIndex = 1;
	for (int x = 0; x < numberOfDefiningLines; x++)
	{
		ECBPolyPoint currentPointA = points[currentPointAIndex];
		ECBPolyPoint currentPointB = points[currentPointBIndex];
		if (debugFlag == 1)
		{
			std::cout << "Debugging Y-rotate, point A: " << currentPointA.x << ", " << currentPointA.y << ", " << currentPointA.z << std::endl;
			std::cout << "Debugging Y-rotate, point B: " << currentPointB.x << ", " << currentPointB.y << ", " << currentPointB.z << std::endl;
		}
		if
			(
			(
				(currentPointA.y >= centeredPoint.y)		// is the centered point y between the y of point A and B? 
				&&
				(currentPointB.y <= centeredPoint.y)
				)
				)
		{
			if (debugFlag == 1)
			{
				std::cout << "Y-axis begin --> Valid line found." << std::endl;
				std::cout << "Point A index: " << currentPointAIndex << std::endl;
				std::cout << "Point B index: " << currentPointBIndex << std::endl;
			}
			findPointBForYAxisRotationTheta(currentPointA, currentPointB, centeredPoint);
			break;
		}
		else if
			(
			(
				(currentPointA.y <= centeredPoint.y)		// vice-versa
				&&
				(currentPointB.y >= centeredPoint.y)
				)
				)
		{
			if (debugFlag == 1)
			{
				std::cout << "Y-axis begin --> Valid line found." << std::endl;
				std::cout << "Point A index: " << currentPointAIndex << std::endl;
				std::cout << "Point B index: " << currentPointBIndex << std::endl;
			}
			findPointBForYAxisRotationTheta(currentPointA, currentPointB, centeredPoint);
			break;
		}
		currentPointAIndex++;
		currentPointBIndex++;
	}
}

void OrganicPolygon::findPointBForXAxisRotationTheta(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_centeredPoint)
{
	// finds the value of "point B" in the calculation for getting the theta of points ABC.
	// -point A is the center (centeredPoint), 
	// -point B is this point, 
	// -point C is a point where the z value is equal to that of point A, but it's y-value is that of point B. (point C in calculateRadianForXRotation)
	//std::cout << "(X-rotation) Range-check is valid, Calculating!!!! " << std::endl;
	ECBPolyPoint slope;
	if (in_pointB.x >= in_centeredPoint.x)
	{
		slope.x = in_pointB.x - in_pointA.x;	// calculate base slope coords
		slope.y = in_pointB.y - in_pointA.y;
		slope.z = in_pointB.z - in_pointA.z;
		float squared_distance_to_x = pow(slope.x, 2.0f);
		float squared_distance_to_y = pow(slope.y, 2.0f);
		float squared_distance_to_z = pow(slope.z, 2.0f);
		float fullLineLength = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		float travelDistanceForXMatch = in_centeredPoint.x / in_pointB.x;	// don't forget to account for dividing by 0!!!!
		//xAxisThetaPointB.x = in_pointA.x + (slope.x * travelDistanceForYMatch);	

		float origin_to_border_x_diff = (in_centeredPoint.x - in_pointA.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
		float time_to_complete_x_traversal = origin_to_border_x_diff / slope.x;				// get the distance that the ray has to travel to get to this value of x


		xAxisThetaPointB.x = in_centeredPoint.x;
		//xAxisThetaPointB.y = in_pointA.y + (slope.y * travelDistanceForXMatch);
		//xAxisThetaPointB.z = in_pointA.z + (slope.z * travelDistanceForXMatch);

		xAxisThetaPointB.y = in_pointB.y + (slope.y * time_to_complete_x_traversal);	// fix for 12/18/2018
		xAxisThetaPointB.z = in_pointB.z + (slope.z * time_to_complete_x_traversal);

		if (debugFlag == 1)
		{
			std::cout << "!! Case 1" << std::endl;
			std::cout << "Point A is: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;
			std::cout << "Point B is: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
			std::cout << "X-axis begin will be at: " << xAxisThetaPointB.x << ", " << xAxisThetaPointB.y << ", " << xAxisThetaPointB.z << ", " << std::endl;
		}
	}
	else if (in_pointB.x <= in_centeredPoint.x)
	{
		slope.x = in_pointA.x - in_pointB.x;	// calculate base slope coords
		slope.y = in_pointA.y - in_pointB.y;
		slope.z = in_pointA.z - in_pointB.z;
		float squared_distance_to_x = pow(slope.x, 2.0f);
		float squared_distance_to_y = pow(slope.y, 2.0f);
		float squared_distance_to_z = pow(slope.z, 2.0f);
		float fullLineLength = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		float travelDistanceForXMatch = in_centeredPoint.x / in_pointA.x;	// don't forget to account for dividing by 0!!!!

		float origin_to_border_x_diff = (in_centeredPoint.x - in_pointB.x);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
		float time_to_complete_x_traversal = origin_to_border_x_diff / slope.x;				// get the distance that the ray has to travel to get to this value of x

		//xAxisThetaPointB.x = in_pointB.x + (slope.x * travelDistanceForYMatch);	
		xAxisThetaPointB.x = in_centeredPoint.x;
		//xAxisThetaPointB.y = in_pointB.y + (slope.y * travelDistanceForXMatch);
		//xAxisThetaPointB.z = in_pointB.z + (slope.z * travelDistanceForXMatch);

		xAxisThetaPointB.y = in_pointB.y + (slope.y * time_to_complete_x_traversal);	// fix for 12/18/2018
		xAxisThetaPointB.z = in_pointB.z + (slope.z * time_to_complete_x_traversal);

		if (debugFlag == 1)
		{
			std::cout << "!! Case 2" << std::endl;
			std::cout << "Point A is: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;
			std::cout << "Point B is: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
			std::cout << "X-axis begin will be at: " << xAxisThetaPointB.x << ", " << xAxisThetaPointB.y << ", " << xAxisThetaPointB.z << ", " << std::endl;
		}
	}
}

void OrganicPolygon::findPointBForYAxisRotationTheta(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_centeredPoint)
{
	//std::cout << "(Y-rotation) Range-check is valid, Calculating!!!! " << std::endl;
	ECBPolyPoint slope;
	if (in_pointB.y >= in_centeredPoint.y)
	{
		slope.x = in_pointB.x - in_pointA.x;	// calculate base slope coords
		slope.y = in_pointB.y - in_pointA.y;
		slope.z = in_pointB.z - in_pointA.z;
		float squared_distance_to_x = pow(slope.x, 2.0f);
		float squared_distance_to_y = pow(slope.y, 2.0f);
		float squared_distance_to_z = pow(slope.z, 2.0f);
		float fullLineLength = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		float travelDistanceForYMatch = in_centeredPoint.y / in_pointB.y;	// don't forget to account for dividing by 0!!!!

		float origin_to_border_y_diff = (in_centeredPoint.y - in_pointA.y);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
		float time_to_complete_y_traversal = origin_to_border_y_diff / slope.y;				// get the distance that the ray has to travel to get to this value of x

		//yAxisThetaPointB.x = in_pointA.x + (slope.x * travelDistanceForYMatch);	// calculate the begin point
		//yAxisThetaPointB.y = in_pointA.y + (slope.y * travelDistanceForYMatch);
		yAxisThetaPointB.y = in_centeredPoint.y;
		//yAxisThetaPointB.z = in_pointA.z + (slope.z * travelDistanceForYMatch);

		yAxisThetaPointB.x = in_pointA.x + (slope.x * time_to_complete_y_traversal);	// fix for 12/18/2018
		yAxisThetaPointB.z = in_pointA.z + (slope.z * time_to_complete_y_traversal);

		//std::cout << "Y-axis begin will be at: " << yAxisThetaPointB.x << ", " << yAxisThetaPointB.y << ", " << yAxisThetaPointB.z << ", " << std::endl;
	}
	else if (in_pointB.y <= in_centeredPoint.y)
	{
		//std::cout << "||| Y-axis centered point: " << in_centeredPoint.x << ", " << in_centeredPoint.y << ", " << in_centeredPoint.z << std::endl;
		//std::cout << "||| Point A: " << in_pointA.x << ", " << in_pointA.y << ", " << in_pointA.z << std::endl;

		slope.x = in_pointA.x - in_pointB.x;	// calculate base slope coords
		slope.y = in_pointA.y - in_pointB.y;
		slope.z = in_pointA.z - in_pointB.z;
		float squared_distance_to_x = pow(slope.x, 2.0f);
		float squared_distance_to_y = pow(slope.y, 2.0f);
		float squared_distance_to_z = pow(slope.z, 2.0f);
		float fullLineLength = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
		float travelDistanceForYMatch = in_centeredPoint.y / in_pointA.y;	// don't forget to account for dividing by 0!!!!

		float origin_to_border_y_diff = (in_centeredPoint.y - in_pointB.y);					// this value represents what we need to multiply y and z by in order to get the distance to the border (pythagorean theorem again)
		float time_to_complete_y_traversal = origin_to_border_y_diff / slope.y;				// get the distance that the ray has to travel to get to this value of x


		//yAxisThetaPointB.x = in_pointB.x + (slope.x * travelDistanceForYMatch);	// calculate the begin point
		//yAxisThetaPointB.y = in_pointB.y + (slope.y * travelDistanceForYMatch);
		yAxisThetaPointB.y = in_centeredPoint.y;
		//yAxisThetaPointB.z = in_pointB.z + (slope.z * travelDistanceForYMatch);

		yAxisThetaPointB.x = in_pointA.x + (slope.x * time_to_complete_y_traversal);	// fix for 12/18/2018
		yAxisThetaPointB.z = in_pointA.z + (slope.z * time_to_complete_y_traversal);

		//std::cout << "--> Y-axis begin will be at: " << yAxisThetaPointB.x << ", " << yAxisThetaPointB.y << ", " << yAxisThetaPointB.z << ", " << std::endl;
	}
}

void OrganicPolygon::calculateRadianForXRotation()
{
	ECBPolyPoint pointC;
	pointC.x = centeredPoint.x;
	pointC.y = xAxisThetaPointB.y;
	pointC.z = centeredPoint.z;
	float hypotenuse = findLineLength(centeredPoint, xAxisThetaPointB);
	float adjacent = findLineLength(centeredPoint, pointC);
	xRotationRadian = findRadianViaCAH(adjacent, hypotenuse);
}

void OrganicPolygon::calculateRadianForYRotation()
{
	ECBPolyPoint pointC;
	pointC.x = yAxisThetaPointB.x;
	pointC.y = centeredPoint.y;
	pointC.z = centeredPoint.z;
	float hypotenuse = findLineLength(centeredPoint, yAxisThetaPointB);
	float adjacent = findLineLength(centeredPoint, pointC);
	yRotationRadian = findRadianViaCAH(adjacent, hypotenuse);
}


void OrganicPolygon::rotatePointsAboutXAxis()
{
	setRotationDirectionForX();	// set rotation as being either CW or CCW
	for (int x = 0; x < numberOfPoints; x++)
	{
		//std::cout << "Current Z for point " << x << " is: " << currentZ << std::endl;
		ECBPolyPoint rotationOrigin;
		rotationOrigin = centeredPoint;

		//rotationPointA.x = points[x].x;
		//rotationPointA.y = centeredPoint.y;
		//rotationPointA.z = currentZ;
		points[x] = rotatePointAboutX(rotationOrigin, points[x], xRotationRadian); // replace the point in the array with the result of it being rotated along the x-axis
	}
}

void OrganicPolygon::rotatePointsAboutYAxis()
{
	setRotationDirectionForY();
	for (int x = 0; x < numberOfPoints; x++)
	{
		ECBPolyPoint rotationOrigin;
		rotationOrigin = centeredPoint;
		points[x] = rotatePointAboutY(rotationOrigin, points[x], yRotationRadian);
	}
}


float OrganicPolygon::findLineLength(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB)
{
	float length = 0.0f;
	ECBPolyPoint slope;
	slope.x = in_pointB.x - in_pointA.x;
	slope.y = in_pointB.y - in_pointA.y;
	slope.z = in_pointB.z - in_pointA.z;
	float squared_distance_to_x = pow(slope.x, 2.0f);
	float squared_distance_to_y = pow(slope.y, 2.0f);
	float squared_distance_to_z = pow(slope.z, 2.0f);
	length = sqrt(squared_distance_to_x + squared_distance_to_y + squared_distance_to_z);
	return length;
}

void OrganicPolygon::setRotationDirectionForX()
{
	//std::cout << "x-theta pointB: " << xAxisThetaPointB.x << ", " << xAxisThetaPointB.y << ", " << xAxisThetaPointB.z << std::endl;

	if (centeredPoint.z > xAxisThetaPointB.z)		// plan for CW rotation
	{
		if (centeredPoint.y < xAxisThetaPointB.y)
		{
			//std::cout << ">>>> selecting CW for X!" << std::endl;
			rotationDirectionX = 1;	// CW 
		}
		else if (centeredPoint.y > xAxisThetaPointB.y)
		{
			//std::cout << ">>>> selecting CCW for X!" << std::endl;
			rotationDirectionX = 0;	// CCW
		}
	}
	else if (centeredPoint.z < xAxisThetaPointB.z)
	{
		if (centeredPoint.y < xAxisThetaPointB.y)
		{
			//std::cout << ">>>> selecting CCW for X!" << std::endl;
			rotationDirectionX = 0;	// CCW
		}
		else if (centeredPoint.y > xAxisThetaPointB.y)
		{
			//std::cout << ">>>> selecting CW for X!" << std::endl;
			rotationDirectionX = 1;	// CW 
		}
	}
}

void OrganicPolygon::setRotationDirectionForY()
{
	if (centeredPoint.z > yAxisThetaPointB.z)
	{
		if (centeredPoint.x < yAxisThetaPointB.x)
		{
			rotationDirectionY = 1;	// CW
		}
		else if (centeredPoint.x < yAxisThetaPointB.x)
		{
			rotationDirectionY = 0;	// CCW
		}
	}
	else if (centeredPoint.z < yAxisThetaPointB.z)
	{
		if (centeredPoint.x < yAxisThetaPointB.x)
		{
			rotationDirectionY = 0;	// CCW
		}
		else if (centeredPoint.x > yAxisThetaPointB.x)
		{
			rotationDirectionY = 1;	// CW
		}
	}
}

float OrganicPolygon::findRadianViaCAH(float in_adjacent, float in_hypotenuse)
{
	return acos(in_adjacent / in_hypotenuse);
}

ECBPolyPoint OrganicPolygon::rotatePointAboutX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float in_radians)
{
	ECBPolyPoint rotatedPoint;
	ECBPolyPoint translationForOrigin;

	// compute the translation for the origin
	if (in_originPoint.x != 0.0f)
	{
		translationForOrigin.x = -in_originPoint.x;
	}
	if (in_originPoint.y != 0.0f)
	{
		translationForOrigin.y = -in_originPoint.y;
	}
	if (in_originPoint.z != 0.0f)
	{
		translationForOrigin.z = -in_originPoint.z;
	}

	// translate origin and PointB
	ECBPolyPoint translated_origin;
	translated_origin.x = in_originPoint.x + translationForOrigin.x;
	translated_origin.y = in_originPoint.y + translationForOrigin.y;
	translated_origin.z = in_originPoint.z + translationForOrigin.z;
	ECBPolyPoint translated_pointB;
	translated_pointB.x = in_pointB.x + translationForOrigin.x;
	translated_pointB.y = in_pointB.y + translationForOrigin.y;
	translated_pointB.z = in_pointB.z + translationForOrigin.z;

	if (debugFlag == 1)
	{
		std::cout << "Radian value: " << in_radians << std::endl;
	}
	// if z of pointB is less than the origin (after translation), rotate CW
	//std::cout << "translated origin: " << translated_origin.x << ", " << translated_origin.y << ", " << translated_origin.z << std::endl;
	//std::cout << "translated pointB: " << translated_pointB.x << ", " << translated_pointB.y << ", " << translated_pointB.z << std::endl;

	if (rotationDirectionX == 0)	// all points will be rotated CW
	{
		//std::cout << "X: rotating CCW" << std::endl;
		rotatedPoint = rotateLineCCWAlongX(translated_origin, translated_pointB, in_radians);
	}
	else if (rotationDirectionX == 1) // all points will be rotation CCW
	{
		//std::cout << "X: rotating CW" << std::endl;
		rotatedPoint = rotateLineCWAlongX(translated_origin, translated_pointB, in_radians);
	}

	// reverse translation
	rotatedPoint.x += -translationForOrigin.x;
	rotatedPoint.y += -translationForOrigin.y;
	rotatedPoint.z += -translationForOrigin.z;

	if (debugFlag == 1)
	{
		std::cout << "+++++++++++++++++++ Calculated points (X-rotation) ++++++++++++++++++" << std::endl;
		std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
		std::cout << "Pre-rotated point: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
		std::cout << "Rotated point: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	}
	return rotatedPoint;
}

ECBPolyPoint OrganicPolygon::rotatePointAboutY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float in_radians)
{
	ECBPolyPoint rotatedPoint;
	ECBPolyPoint translationForOrigin;
	// compute the translation for the origin
	if (in_originPoint.x != 0.0f)
	{
		translationForOrigin.x = -in_originPoint.x;
	}
	if (in_originPoint.y != 0.0f)
	{
		translationForOrigin.y = -in_originPoint.y;
	}
	if (in_originPoint.z != 0.0f)
	{
		translationForOrigin.z = -in_originPoint.z;
	}

	// translate origin and PointB
	ECBPolyPoint translated_origin;
	translated_origin.x = in_originPoint.x + translationForOrigin.x;
	translated_origin.y = in_originPoint.y + translationForOrigin.y;
	translated_origin.z = in_originPoint.z + translationForOrigin.z;
	ECBPolyPoint translated_pointB;
	translated_pointB.x = in_pointB.x + translationForOrigin.x;
	translated_pointB.y = in_pointB.y + translationForOrigin.y;
	translated_pointB.z = in_pointB.z + translationForOrigin.z;

	// if z of pointB is less than the origin (after translation), rotate CW

	if (rotationDirectionY == 0)	// all points will be rotated CW
	{
		rotatedPoint = rotateLineCCWAlongY(translated_origin, translated_pointB, in_radians);
	}
	else if (rotationDirectionY == 1) // all points will be rotation CCW
	{
		rotatedPoint = rotateLineCWAlongY(translated_origin, translated_pointB, in_radians);
	}

	// reverse translation
	rotatedPoint.x += -translationForOrigin.x;
	rotatedPoint.y += -translationForOrigin.y;
	rotatedPoint.z += -translationForOrigin.z;
	if (debugFlag == 1)
	{
		std::cout << "+++++++++++++++++++ Calculated points (Y-rotation) ++++++++++++++++++" << std::endl;
		std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
		std::cout << "Pre-rotated point: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
		std::cout << "Rotated point: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	}

	return rotatedPoint;
}

ECBPolyPoint OrganicPolygon::rotateLineCWAlongX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians)	// rotates a line counter clock wise
{
	ECBPolyPoint rotatedPoint;
	rotatedPoint = in_pointB;
	ECBPolyPoint preRotatedPoint = in_pointB;
	float radian360 = 2 * POLYPI;		// two pi = 6.28 radians
	//std::cout << "CCW - radian360: " << radian360 << std::endl;
	//std::cout << "CCW - radian parameter: " << radians << std::endl;
	//std::cout << "--------------------------------------------" << std::endl;
	//std::cout << "(X) CW - radian parameter: " << radians << std::endl;
	//std::cout << "> Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "> Point B: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	float valueToCheck = radians;
	valueToCheck = radian360 - valueToCheck;
	rotatedPoint.x = in_pointB.x;
	rotatedPoint.z = ((preRotatedPoint.z)*cos(valueToCheck)) - ((preRotatedPoint.y)*sin(valueToCheck));		// x' = xcos(theta) - ysin(theta)		(z is used as x axis)
	rotatedPoint.y = ((preRotatedPoint.z)*sin(valueToCheck)) + ((preRotatedPoint.y)*cos(valueToCheck));		// y' = xsin(theta) - ycos(theta)		(y is used as y axis)
	//std::cout << "> Point C: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	return rotatedPoint;
}

ECBPolyPoint OrganicPolygon::rotateLineCWAlongY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians)	// rotates a line counter clock wise
{
	ECBPolyPoint rotatedPoint;
	rotatedPoint = in_pointB;
	ECBPolyPoint preRotatedPoint = in_pointB;
	float radian360 = 2 * POLYPI;		// two pi = 6.28 radians
	//std::cout << "CCW - radian360: " << radian360 << std::endl;
	//std::cout << "CCW - radian parameter: " << radians << std::endl;
	//std::cout << "--------------------------------------------" << std::endl;
	//std::cout << "(X) CCW - radian parameter: " << radians << std::endl;
	//std::cout << "> Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "> Point B: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	float valueToCheck = radians;
	valueToCheck = radian360 - valueToCheck;
	rotatedPoint.y = in_pointB.y;
	rotatedPoint.z = ((preRotatedPoint.z)*cos(valueToCheck)) - ((preRotatedPoint.x)*sin(valueToCheck));		// x' = xcos(theta) - ysin(theta)		(z is used as x axis)
	rotatedPoint.x = ((preRotatedPoint.z)*sin(valueToCheck)) + ((preRotatedPoint.x)*cos(valueToCheck));		// y' = xsin(theta) - ycos(theta)		(x is used as y axis)?
	//std::cout << "> Point C: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	return rotatedPoint;
}

ECBPolyPoint OrganicPolygon::rotateLineCCWAlongX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians)
{
	ECBPolyPoint rotatedPoint;
	rotatedPoint = in_pointB;
	ECBPolyPoint preRotatedPoint = in_pointB;
	//float radian360 = 2 * PI;		// two pi = 6.28 radians
	//std::cout << "CW - radian360: " << radian360 << std::endl;
	//std::cout << "--------------------------------------------" << std::endl;
	//std::cout << "(X) CCW - radian parameter: " << radians << std::endl;
	//std::cout << "> Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "> Point B: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	float valueToCheck = radians;
	//valueToCheck = radian360 - valueToCheck;
	rotatedPoint.x = in_pointB.x;
	rotatedPoint.z = ((preRotatedPoint.z)*cos(valueToCheck)) - ((preRotatedPoint.y)*sin(valueToCheck));		// x' = xcos(theta) - ysin(theta)		(z is used as x axis)
	rotatedPoint.y = ((preRotatedPoint.z)*sin(valueToCheck)) + ((preRotatedPoint.y)*cos(valueToCheck));		// y' = xsin(theta) - ycos(theta)		(y is used as y axis)

	//std::cout << "> Point C: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	return rotatedPoint;
}

ECBPolyPoint OrganicPolygon::rotateLineCCWAlongY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians)	// rotates a line counter clock wis
{
	ECBPolyPoint rotatedPoint;
	rotatedPoint = in_pointB;
	ECBPolyPoint preRotatedPoint = in_pointB;
	//float radian360 = 2 * PI;		// two pi = 6.28 radians
	//std::cout << "CW - radian360: " << radian360 << std::endl;
	//std::cout << "--------------------------------------------" << std::endl;
	//std::cout << "(Y) CW - radian parameter: " << radians << std::endl;
	//std::cout << "> Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "> Point B: " << in_pointB.x << ", " << in_pointB.y << ", " << in_pointB.z << std::endl;
	float valueToCheck = radians;
	//valueToCheck = radian360 - valueToCheck;
	rotatedPoint.y = in_pointB.y;
	rotatedPoint.z = ((preRotatedPoint.z)*cos(valueToCheck)) - ((preRotatedPoint.x)*sin(valueToCheck));		// x' = xcos(theta) - ysin(theta)		(z is used as x axis)
	rotatedPoint.x = ((preRotatedPoint.z)*sin(valueToCheck)) + ((preRotatedPoint.x)*cos(valueToCheck));		// y' = xsin(theta) - ycos(theta)		(x is used as y axis)?

	//std::cout << "> Point C: " << rotatedPoint.x << ", " << rotatedPoint.y << ", " << rotatedPoint.z << ", " << std::endl;
	return rotatedPoint;
}

void OrganicPolygon::rotateTo2D()
{
	// setup the center point
	calculateCenteredPoint();

	// rotate on x-axis
	findXAxisRotationPointB();
	calculateRadianForXRotation();
	rotatePointsAboutXAxis();

	//std::cout << "--------------BEGIN NEXT ROTATION LOGIC----------------" << std::endl;

	// rotate on y-axis
	findYAxisRotationPointB();
	calculateRadianForYRotation();
	rotatePointsAboutYAxis();
}

void OrganicPolygon::findHighestXYDiffs()
{
	for (int x = 0; x < numberOfPoints; x++)
	{
		// for everything but the last point: get x/y diff of the next point, minus this one
		if (x != (numberOfPoints - 1))
		{
			float currentXDiff = points[x + 1].x - points[x].x;
			float currentYDiff = points[x + 1].y - points[x].y;
			if (currentXDiff > highestXDiff)
			{
				highestXDiff = currentXDiff;
			}
			if (currentYDiff > highestYDiff)
			{
				highestYDiff = currentYDiff;
			}
		}
		else if (x == (numberOfPoints - 1))
		{
			float currentXDiff = points[0].x - points[x].x;
			float currentYDiff = points[0].y - points[x].y;
			if (currentXDiff > highestXDiff)
			{
				highestXDiff = currentXDiff;
			}
			if (currentYDiff > highestYDiff)
			{
				highestYDiff = currentYDiff;
			}
		}
		// the last point will need to be compared to the first point (so point 0 - last point)
	}

	//std::cout << "Highest x diff is: " << highestXDiff << std::endl;
	//std::cout << "Highest y diff is: " << highestYDiff << std::endl;
}

void OrganicPolygon::findHighestXYDiffs2()
{
	//highestXDiff = 0.0f;	// reset to 0, if it wasn't already
	//highestYDiff = 0.0f;	// ""
	for (int x = 0; x < numberOfPoints; x++)
	{
		float currentXStretch = getXBoundaryStretch(points[x].x);
		if (currentXStretch > highestXDiff)
		{
			highestXDiff = currentXStretch;
		}

		float currentYStretch = getXBoundaryStretch(points[x].y);
		if (currentYStretch > highestYDiff)
		{
			highestYDiff = currentYStretch;
		}
	}
}

void OrganicPolygon::findHighestXYDiffs3()
{
	float distanceToXLimitPos = 1.0f - centeredPoint.x;  // if 1.0f x is the limit, subtract the current centeredPoint from it
	float maxXLimitPos = distanceToXLimitPos;					// by default, this is equal to the distanceXLimit

	float distanceToYLimitPos = 1.0f - centeredPoint.y;
	float maxYLimitPos = distanceToYLimitPos;

	for (int x = 0; x < numberOfPoints; x++)
	{
		if (abs(points[x].x - centeredPoint.x) > maxXLimitPos)
		{
			maxXLimitPos = abs(points[x].x - centeredPoint.x);
			if (debugFlag == 1)
			{
				std::cout << "******NEW x limit obtained: " << std::endl;
				std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
				std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
				std::cout << "-> X limit: " << maxXLimitPos << std::endl;
			}
		}

		if (abs(points[x].y - centeredPoint.y) > maxYLimitPos)
		{
			maxYLimitPos = abs(points[x].y - centeredPoint.y);
			if (debugFlag == 1)
			{
				std::cout << "******NEW y limit obtained: " << std::endl;
				std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
				std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
				std::cout << "-> Y limit: " << maxYLimitPos << std::endl;
			}
		}

	}
	xDiffMultiplierPos = distanceToXLimitPos / maxXLimitPos;		// find the multiplier for X
	yDiffMultiplierPos = distanceToYLimitPos / maxYLimitPos;		// "" y
	if (debugFlag == 1)
	{
		std::cout << "maxXLimitPos = " << maxXLimitPos << std::endl;
		std::cout << "maxYLimitPos = " << maxYLimitPos << std::endl;

		std::cout << "||----> xMultiplier: (Positive)" << xDiffMultiplierPos << std::endl;
		std::cout << "||----> yMultiplier: (Positive)" << yDiffMultiplierPos << std::endl;
	}
}

void OrganicPolygon::findHighestXYDiffs4()
{
	// X limits
	float distanceToXLimitPos = 1.0f - centeredPoint.x;  // if 1.0f x is the limit, subtract the current centeredPoint from it
	float maxXLimitPos = distanceToXLimitPos;					// by default, this is equal to the distanceXLimit

	float distanceToXLimitNeg = abs(0.0f - centeredPoint.x);
	float maxXLimitNeg = distanceToXLimitNeg;


	// Y limits
	float distanceToYLimitPos = 1.0f - centeredPoint.y;
	float maxYLimitPos = distanceToYLimitPos;

	float distanceToYLimitNeg = abs(0.0f - centeredPoint.y);
	float maxYLimitNeg = distanceToYLimitNeg;

	for (int x = 0; x < numberOfPoints; x++)
	{
		// X Calcs
		if (points[x].x > centeredPoint.x)
		{
			if (abs(points[x].x - centeredPoint.x) > maxXLimitPos)
			{
				maxXLimitPos = abs(points[x].x - centeredPoint.x);
				if (debugFlag == 1)
				{
					std::cout << "******NEW Positive x limit obtained: " << std::endl;
					std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
					std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
					std::cout << "-> X limit: " << maxXLimitPos << std::endl;
				}
			}
		}
		else if (points[x].x < centeredPoint.x)
		{
			if (abs(points[x].x - centeredPoint.x) > maxXLimitNeg)
			{
				maxXLimitNeg = abs(points[x].x - centeredPoint.x);
				if (debugFlag == 1)
				{
					std::cout << "******NEW Negative x limit obtained: " << std::endl;
					std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
					std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
					std::cout << "-> X limit: " << maxXLimitNeg << std::endl;
				}
			}
		}

		// Y Calcs
		if (points[x].y > centeredPoint.y)
		{
			if (abs(points[x].y - centeredPoint.y) > maxYLimitPos)
			{
				maxYLimitPos = abs(points[x].y - centeredPoint.y);
				if (debugFlag == 1)
				{
					std::cout << "******NEW Positive y limit obtained: " << std::endl;
					std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
					std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
					std::cout << "-> Y limit: " << maxYLimitPos << std::endl;
				}
			}
		}
		else if (points[x].y < centeredPoint.y)
		{
			if (abs(points[x].y - centeredPoint.y) > maxYLimitNeg)
			{
				maxYLimitNeg = abs(points[x].y - centeredPoint.y);
				if (debugFlag == 1)
				{
					std::cout << "******NEW Negative y limit obtained: " << std::endl;
					std::cout << "-> Point A: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
					std::cout << "-> Point B: " << points[x].x << ", " << points[x].y << ", " << points[x].z << std::endl;
					std::cout << "-> Y limit: " << maxYLimitNeg << std::endl;
				}
			}
		}

	}
	xDiffMultiplierPos = distanceToXLimitPos / maxXLimitPos;		// find the multiplier for X
	yDiffMultiplierPos = distanceToYLimitPos / maxYLimitPos;		// "" y

	xDiffMultiplierNeg = distanceToXLimitNeg / maxXLimitNeg;
	yDiffMultiplierNeg = distanceToYLimitNeg / maxYLimitNeg;

	if (debugFlag == 1)
	{
		std::cout << "|||||--> Positive limits: " << std::endl;
		std::cout << "maxXLimitPos = " << maxXLimitPos << std::endl;
		std::cout << "maxYLimitPos = " << maxYLimitPos << std::endl;

		std::cout << "||----> xMultiplier: (Positive)" << xDiffMultiplierPos << std::endl;
		std::cout << "||----> yMultiplier: (Positive)" << yDiffMultiplierPos << std::endl;

		std::cout << "|||||--> Negative limits: " << std::endl;
		std::cout << "maxXLimitNeg = " << maxXLimitNeg << std::endl;
		std::cout << "maxYLimitNeg = " << maxYLimitNeg << std::endl;

		std::cout << "||----> xMultiplier: (Negative)" << xDiffMultiplierNeg << std::endl;
		std::cout << "||----> yMultiplier: (Negative)" << yDiffMultiplierNeg << std::endl;
	}
}

float OrganicPolygon::getXBoundaryStretch(float in_xValue)
{
	if (in_xValue > 1.0f)
	{
		return(in_xValue - 1.0f);	// if we are above 1.0, return the difference between the input and 1.0
	}
	else if (in_xValue < 0.0f)
	{
		return(abs(in_xValue));		// if we are in negative x, just return the absolute of it
	}
	else
	{
		return 0.0f;	// ++should never return, makes compiler happy (won't produce a warning)
	}
}

void OrganicPolygon::checkForDownScale(int in_debugFlag)
{
	// debug only, let's output the original points
	//std::cout << "~~~~Original points:~~~~" << std::endl;
	for (int x = 0; x < numberOfPoints; x++)
	{
		if (in_debugFlag == 1)
		{
			std::cout << "Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}


	float downScaleX = 1.0f;	// default; no scaling required
	float downScaleY = 1.0f;	// ""
	if (highestXDiff >= 1.0f)	// polygon is stretched out of tile range, will need to down scale
	{

		downScaleX /= highestXDiff;
		if (in_debugFlag == 1)
		{
			std::cout << ">>>> Downscaling for X " << std::endl;
			std::cout << ">>>> Value: " << downScaleX << std::endl;
		}
	}
	if (highestYDiff >= 1.0f)
	{
		downScaleY /= highestYDiff;
		if (in_debugFlag == 1)
		{
			std::cout << ">>>> Downscaling for Y" << std::endl;
			std::cout << ">>>> Value: " << downScaleY << std::endl;
		}
	}

	// down scale for x
	for (int x = 0; x < numberOfPoints; x++)
	{
		float currentSlopeX = (points[x].x - centeredPoint.x) * downScaleX; // get the slope's x, from the current point - center point; divide the result by downScaleX
		float currentSlopeY = (points[x].y - centeredPoint.y) * downScaleY; // "" y

		points[x].x = centeredPoint.x + currentSlopeX;
		points[x].y = centeredPoint.y + currentSlopeY;
	}
	//std::cout << "~~~~New points, after X/Y:~~~~" << std::endl;
	for (int x = 0; x < numberOfPoints; x++)
	{
		if (in_debugFlag == 1)
		{
			std::cout << "Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}


}

void OrganicPolygon::checkForDownScale2(int in_debugFlag)
{
	if (in_debugFlag == 1)
	{
		std::cout << "Highest x diff: " << highestXDiff << std::endl;
		std::cout << "Highest y diff: " << highestYDiff << std::endl;
		std::cout << "Origin point is: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
		std::cout << "~~~~Pre downscale points:~~~~" << std::endl;
		for (int x = 0; x < numberOfPoints; x++)
		{
			std::cout << "Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;

		}


	}

	/*
	for (int x = 0; x < numberOfPoints; x++)
	{
		// calcs for X
		if (points[x].x > centeredPoint.x)
		{
			points[x].x -= highestXDiff;
		}
		else if (points[x].x < centeredPoint.x)
		{
			points[x].x += highestXDiff;
		}

		// calcs for Y
		if (points[x].y > centeredPoint.y)
		{
			points[x].y -= highestYDiff;
		}
		else if (points[x].y < centeredPoint.y)
		{
			points[x].y += highestYDiff;
		}

		if (in_debugFlag == 1)
		{
			std::cout << "Downscaled Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}
	*/


	for (int x = 0; x < numberOfPoints; x++)
	{
		if (debugFlag == 1)
		{
			std::cout << "Stats for point " << x << std::endl;
		}

		// x calcs
		//if (points[x].x > centeredPoint.x)
		//{
			//points[x].x *= xDiffMultiplier;
		float currentXDiff = points[x].x - centeredPoint.x;
		float multipliedXDiff = currentXDiff * xDiffMultiplierPos;
		points[x].x = centeredPoint.x + multipliedXDiff;
		//}
		//else if (points[x].x < centeredPoint.x)
		//{
		//	float currentXDiff = points[x].x - centeredPoint.x;
		//	float multipliedXDiff = currentXDiff * xDiffMultiplier;
		//	points[x].x = centeredPoint.x + multipliedXDiff;

		//}

		// y calcs
		//points[x].y *= yDiffMultiplier;
		//if (points[x].y > centeredPoint.y)
		//{


		float currentYDiff = points[x].y - centeredPoint.y;
		float multipliedYDiff = currentYDiff * yDiffMultiplierPos;
		if (debugFlag == 1)
		{
			std::cout << "Current y diff: " << currentYDiff << std::endl;
			std::cout << "yDiffMultiplier: " << yDiffMultiplierPos << std::endl;
			std::cout << "multipledYDiff: " << multipliedYDiff << std::endl;

		}
		points[x].y = centeredPoint.y + multipliedYDiff;
		//}


		if (in_debugFlag == 1)
		{
			std::cout << "Downscaled Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}

}

void OrganicPolygon::checkForDownScale3(int in_debugFlag)
{
	if (in_debugFlag == 1)
	{
		//std::cout << "Highest x diff: " << highestXDiff << std::endl;
		//std::cout << "Highest y diff: " << highestYDiff << std::endl;
		std::cout << "|||||||||||| -------> Origin point is: " << centeredPoint.x << ", " << centeredPoint.y << ", " << centeredPoint.z << std::endl;
		std::cout << "|||||||||||| -------> Pre downscale points:~~~~" << std::endl;
		for (int x = 0; x < numberOfPoints; x++)
		{
			std::cout << "Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;

		}


	}

	for (int x = 0; x < numberOfPoints; x++)
	{
		// X Checks
		if (points[x].x > centeredPoint.x)	// apply xDiffMultiplierPos
		{
			float currentXDiff = points[x].x - centeredPoint.x;
			float multipliedXDiff = currentXDiff * xDiffMultiplierPos;
			points[x].x = centeredPoint.x + multipliedXDiff;
		}
		else if (points[x].x < centeredPoint.x)	 // apply xDiffMultiplierNeg
		{
			float currentXDiff = points[x].x - centeredPoint.x;
			float multipliedXDiff = currentXDiff * xDiffMultiplierNeg;
			points[x].x = centeredPoint.x + multipliedXDiff;
		}




		// Y Checks
		/**/
		if (points[x].y > centeredPoint.y)
		{
			float currentYDiff = points[x].y - centeredPoint.y;
			float multipliedYDiff = currentYDiff * yDiffMultiplierPos;
			points[x].y = centeredPoint.y + multipliedYDiff;
		}
		else if (points[x].y < centeredPoint.y)
		{
			float currentYDiff = points[x].y - centeredPoint.y;
			float multipliedYDiff = currentYDiff * yDiffMultiplierNeg;
			points[x].y = centeredPoint.y + multipliedYDiff;
		}

		if (in_debugFlag == 1)
		{
			std::cout << "Downscaled Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}

	// perform further downscaling (potentially obsolete as of 4/19/2019)
	/*
	for (int x = 0; x < numberOfPoints; x++)
	{
		// x calcs
		float borderDiff = 0.01f;
		if (points[x].x < .5f)
		{
			if ((points[x].x >= 0.0f) && (points[x].x < borderDiff))
			{
				points[x].x = borderDiff;
			}
		}
		else if (points[x].x > .5f)
		{
			if ((points[x].x <= 1.0f) && (points[x].x > (1.0 - borderDiff)))
			{
				points[x].x = (1.0 - borderDiff);
			}
		}

		// y calcs
		if (points[x].y < .5f)
		{
			if ((points[x].y >= 0.0f) && (points[x].y < borderDiff))
			{
				points[x].y = borderDiff;
			}
		}
		else if (points[x].y > .5f)
		{
			if ((points[x].y <= 1.0f) && (points[x].y > (1.0 - borderDiff)))
			{
				points[x].y = (1.0 - borderDiff);
			}
		}

		if (in_debugFlag == 1)
		{
			std::cout << "Border-Downscaled Point " << x << ": " << points[x].x << ", " << points[x].y << std::endl;
		}
	}
	*/
}


ECBPolyPoint OrganicPolygon::getPoint(int in_index)
{
	return points[in_index];
}

void OrganicPolygon::setDebugFlag(int in_debugFlag)
{
	debugFlag = in_debugFlag;
}