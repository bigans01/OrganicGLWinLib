#include "stdafx.h"
#include "LineSegmentIntersectAnalyzer.h"

void LineSegmentIntersectAnalyzer::performAnalysis()
{

	// get the 2D slope for segment A, r
	float segmentASlopeX = lineSegmentA.b.x - lineSegmentA.a.x;
	float segmentASlopeY = lineSegmentA.b.y - lineSegmentA.a.y;
	TwoDPoint r(segmentASlopeX, segmentASlopeY);

	// get the 2D slope for segment B, s
	//std::cout << "subtracting " << lineSegmentB.b.x << " from " << lineSegmentB.a.x << std::endl;
	float segmentBSlopeX = lineSegmentB.b.x - lineSegmentB.a.x;
	float segmentBSlopeY = lineSegmentB.b.y - lineSegmentB.a.y;
	TwoDPoint s(segmentBSlopeX, segmentBSlopeY);




	// *************** Segment A, solve for T (scalar value for Segment A)



	// subtract point A of LineSegment A, from point A of LineSegment B; get the cross
	TwoDPoint preCrossNumerator;
	preCrossNumerator = lineSegmentB.a;

	std::cout << "X/Y of preCrossNumerator: " << preCrossNumerator.x << ",  " << preCrossNumerator.y << std::endl;
	std::cout << "X/Y of s: " << s.x << ", " << s.y << std::endl;

	preCrossNumerator -= lineSegmentA.a;



	float crossNumerator = calculate2DCross(preCrossNumerator, s);
	float crossDenominator = calculate2DCross(r, s);

	std::cout << "crossNumerator is: " << crossNumerator << std::endl;
	std::cout << "crossDenominator is: " << crossDenominator << std::endl;

	float lineSegmentAScalar = crossNumerator / crossDenominator;
	std::cout << "Line Segment A scalar is: " << lineSegmentAScalar << std::endl;
}

float LineSegmentIntersectAnalyzer::calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB)
{
	//glm::vec3 returnVec;

	//returnVec.x = (in_A.y * in_B.z) - (in_A.z * in_B.y);
	//returnVec.y = (in_A.z * in_B.x) - (in_A.x * in_B.z);
	//returnVec.z = (in_A.x * in_B.y) - (in_A.y * in_B.x);

	//return returnVec;

	float crossValue;
	crossValue = (in_crossPointA.x * in_crossPointB.y) - (in_crossPointA.y * in_crossPointB.x);
	return crossValue;
}