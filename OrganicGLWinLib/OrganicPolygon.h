#pragma once

#include <math.h>

#ifndef ORGANICPOLYGON_H
#define ORGANICPOLYGON_H
#define POLYPI 3.14159265f

class OrganicPolygon
{
public:
	void addPoint(ECBPolyPoint in_point);
	void rotateTo2D();
	void findHighestXYDiffs();
	void findHighestXYDiffs2();
	void findHighestXYDiffs3();
	void findHighestXYDiffs4();
	void checkForDownScale(int in_debugFlag);
	void checkForDownScale2(int in_debugFlag);
	void checkForDownScale3(int in_debugFlag);
	void setDebugFlag(int in_debugFlag);
	ECBPolyPoint getPoint(int in_index);
private:
	int debugFlag = 0;		// for debugging purposes only
	int numberOfPoints = 0;
	ECBPolyPoint centeredPoint;	// the point that defines any point that exists within the area of the polygon; could be the actual center of a poly or not
	ECBPolyPoint points[8]; // contains up to 8 points
	float highestXDiff = 0.0f;	// the value of the greatest difference in x (U on the texture) between any two points, after it has been translated to 2d
	float highestYDiff = 0.0f;

	// positive multipliers
	float xDiffMultiplierPos = 1.0f;
	float yDiffMultiplierPos = 1.0f;

	// negative multipliers
	float xDiffMultiplierNeg = 1.0f;
	float yDiffMultiplierNeg = 1.0f;

	void calculateCenteredPoint();
	void setCenteredPointViaTriangleCenter();
	void setCenteredPointViaMidpoint(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);
	float findLineLength(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB);





	// x rotation functions and variables
	void findXAxisRotationPointB();																				// finds the endpoint used to calculate xRotationRadian, known as "xAxisThetaPointB"
	void findPointBForXAxisRotationTheta(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_centeredPoint);	// called by findAlignedxAxisThetaPointBPoint()
	ECBPolyPoint xAxisThetaPointB;																					// the value of "point B" in the calculation for getting the theta of points ABC; set by the call to "findPointBForXAxisRotationTheta
	void calculateRadianForXRotation();																			// uses the centered point and "xAxisThetaPointB" to find the radian for x rotation, known as xRotationRadian
	void setRotationDirectionForX();
	int rotationDirectionX = 0; // 0 = CW, 1 = CCW
	void rotatePointsAboutXAxis();																				// rotates all points in the array on the x-axis, by the value of xRotationRadian.
	ECBPolyPoint rotatePointAboutX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float in_radians);
	ECBPolyPoint rotateLineCWAlongX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians);	// rotates a line counter clock wise
	ECBPolyPoint rotateLineCCWAlongX(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians);	// rotates a line counter clock wise
	float xRotationRadian = 0.0f;																				// the radian value to rotate points along the x-axis by; it is set by calculateRadianForXRotation.



	// y rotation functions and variables
	void findYAxisRotationPointB();
	void findPointBForYAxisRotationTheta(ECBPolyPoint in_pointA, ECBPolyPoint in_pointB, ECBPolyPoint in_centeredPoint);
	ECBPolyPoint yAxisThetaPointB;
	void calculateRadianForYRotation();
	void setRotationDirectionForY();
	int rotationDirectionY = 0;
	void rotatePointsAboutYAxis();
	ECBPolyPoint rotatePointAboutY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float in_radians);
	ECBPolyPoint rotateLineCWAlongY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians);	// rotates a line counter clock wise
	ECBPolyPoint rotateLineCCWAlongY(ECBPolyPoint in_originPoint, ECBPolyPoint in_pointB, float radians);	// rotates a line counter clock wise
	float yRotationRadian = 0.0f;

	float findRadianViaCAH(float in_adjacent, float in_hypotenuse);	// SOHCAHTOA/rotation functions
	float getXBoundaryStretch(float in_xValue);		// highest difference checkers
	int getValidEndpoint();							// used to find the endpoint, used in conjunction with point 0, which will be used to give us the midpoint that contains our value for centeredPoint.



};

#endif


