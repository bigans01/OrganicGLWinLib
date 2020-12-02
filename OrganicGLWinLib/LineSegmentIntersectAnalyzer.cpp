#include "stdafx.h"
#include "LineSegmentIntersectAnalyzer.h"

void LineSegmentIntersectAnalyzer::performAnalysis()
{
	// see this URL: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect


	// get the 2D slope for segment A, r
	//float segmentASlopeX = lineSegmentA.b.x - lineSegmentA.a.x;
	//float segmentASlopeY = lineSegmentA.b.y - lineSegmentA.a.y;
	//TwoDPoint r(segmentASlopeX, segmentASlopeY);
	r = lineSegmentA.b - lineSegmentA.a;


	// get the 2D slope for segment B, s
	//std::cout << "subtracting " << lineSegmentB.b.x << " from " << lineSegmentB.a.x << std::endl;
	//float segmentBSlopeX = lineSegmentB.b.x - lineSegmentB.a.x;
	//float segmentBSlopeY = lineSegmentB.b.y - lineSegmentB.a.y;
	//TwoDPoint s(segmentBSlopeX, segmentBSlopeY);
	s = lineSegmentB.b - lineSegmentB.a;


	// first part of numerator, used in both the t and u solving equations
	TwoDPoint preCrossNumerator;
	preCrossNumerator = lineSegmentB.a;
	preCrossNumerator -= lineSegmentA.a;
	std::cout << "PrecrossNumerator is: " << preCrossNumerator.x << ", " << preCrossNumerator.y << std::endl;
	std::cout << "r is: " << r.x << ", " << r.y << std::endl;
	std::cout << "s is: " << s.x << ", " << s.y << std::endl;

	// *************** Segment A, solve for t (scalar value for Segment A)
	// subtract point A of LineSegment A, from point A of LineSegment B; get the cross
	
	float crossNumeratorA = calculate2DCross(preCrossNumerator, s);
	float crossDenominatorA = calculate2DCross(r, s);

	std::cout << "crossNumeratorA is: " << crossNumeratorA << std::endl;
	std::cout << "crossDenominatorA is: " << crossDenominatorA << std::endl;

	// 0 division protection:
	//float t = crossNumeratorA / crossDenominatorA;						// this should be set as t
	float t = 0.0f;
	if
	(
		(crossNumeratorA != 0.0f)
		&&
		(crossDenominatorA != 0.0f)
	)
	{
		t = crossNumeratorA / crossDenominatorA;
	}
	

	std::cout << "Line Segment A scalar (variable t) is : " << t << std::endl;

	// *************** Segment B, solve for u (scalar value for Segment B)
	// following the referenced equation:  u = (q - p) x r / (r x s); note x = cross.
	// subtract point A of LineSegmentB

	float crossNumeratorB = calculate2DCross(preCrossNumerator, r);
	//std::cout << "!!! (1) Prior to branching, r is : " << r.x << ", " << r.y << std::endl;

	float crossDenominatorB = calculate2DCross(r, s);
	//float u = crossNumeratorB / crossDenominatorB;
	float u = 0.0f;
	if
	(
		(crossNumeratorB != 0.0f)
		&&
		(crossDenominatorB != 0.0f)
	)
	{
		u = crossNumeratorB / crossDenominatorB;
	}

	//std::cout << "!!! (2) Prior to branching, r is : " << r.x << ", " << r.y << std::endl;
	std::cout << "Line Segment B scalar (variable u) is : " << u << std::endl;

	// test the point values 

	// Segment A
	TwoDPoint segmentAStart = lineSegmentA.a;
	TwoDPoint segmentASlopeScaled = (r * t);
	TwoDPoint intersectedCalcPointA = segmentAStart;
	intersectedCalcPointA += segmentASlopeScaled;
	std::cout << "Segment A, intersected point: " << intersectedCalcPointA.x << ", " << intersectedCalcPointA.y << std::endl;

	// Segment B
	TwoDPoint segmentBStart = lineSegmentB.a;
	TwoDPoint segmentBSlopeScaled = (s * u);
	TwoDPoint intersectedCalcPointB = segmentBStart;
	intersectedCalcPointB += segmentBSlopeScaled;
	std::cout << "Segment B, intersected point: " << intersectedCalcPointB.x << ", " << intersectedCalcPointB.y << std::endl;

	float rsCross = calculate2DCross(r, s);

	std::cout << "!!! (3) Prior to branching, r is : " << r.x << ", " << r.y << std::endl;

	if (rsCross == 0.0f)	
	{
		// COLINEAR conditions
		if (calculate2DCross((lineSegmentB.a - lineSegmentA.a), r) == 0.0f)
		{
			if (checkForColinearOverlap() == true)		// COLINEAR_OVERLAP
			{
				std::cout << "Lines are colinear overlap! " << std::endl;
			}
			else   // COLINEAR_NOOVERLAP
			{
				std::cout << "Lines are colinear, and don't overlap! " << std::endl;
			}
			
		}

		// PARALLEL, NON-INTERSECTING
		else if (calculate2DCross((lineSegmentB.a - lineSegmentA.a), r) != 0.0f)	// PARALLEL
		{
			std::cout << "Lines are parallel, but don't intersect! " << std::endl;
		}
		
	}
	else if 
	(
		(rsCross != 0.0f)
		&&
		
		// t
		(t <= 1)
		&&
		(t >= 0)

		&&

		// u
		(u <= 1)
		&&
		(u >= 0)
	)
	{
		// NONCOLINEAR_INTERSECT
		std::cout << "Lines intersect! " << std::endl;
	}
	else
	{
		// NO_INTERSECT
		std::cout << "Lines don't intersect! " << std::endl;
	}

	
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
	//if (crossValue == 0.0f)
	//{
		//std::cout << "!!! Warning: 0 cross detected! " << std::endl;
	//}
	return crossValue;
}

float LineSegmentIntersectAnalyzer::calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB)
{
	//std::cout << "#### pointA: " << in_dotPointA.x << ", " << in_dotPointA.y << std::endl;
	//std::cout << "#### pointB: " << in_dotPointB.x << ", " << in_dotPointB.y << std::endl;

	return (in_dotPointA.x * in_dotPointB.x) + (in_dotPointA.y * in_dotPointB.y);
}

bool LineSegmentIntersectAnalyzer::checkForColinearOverlap()
{
	bool returnValue = false;

	//std::cout << "**** r is: " << r.x << ", " << r.y << std::endl;
	//std::cout << "********** Line segment B, pointA: " << lineSegmentB.a.x << ", " << lineSegmentB.a.y << std::endl;
	//std::cout << "********** Line segment A, pointA: " << lineSegmentA.a.x << ", " << lineSegmentA.a.y << std::endl;

	float t0 = calculate2DDot((lineSegmentB.a - lineSegmentA.a), r) / calculate2DDot(r, r);
	float t1 = calculate2DDot((lineSegmentB.a + s - lineSegmentA.a), r) / calculate2DDot(r, r);

	//std::cout << "t0 is: " << t0 << std::endl;
	//std::cout << "t1 is: " << t1 << std::endl;

	if
	(
		(t0 <= 1)
		&&
		(t0 >= 0)
	)
	{
		returnValue = true;	// overlaps
	}
	return returnValue;
}