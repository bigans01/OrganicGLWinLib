#include "stdafx.h"
#include "LineSegmentIntersectAnalyzer.h"

void LineSegmentIntersectAnalyzer::performAnalysis()
{
	// see this URL: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	r = lineSegmentA.b - lineSegmentA.a;		// get the 2D slope for segment A, r
	s = lineSegmentB.b - lineSegmentB.a;		// get the 2D slope for segment B, s


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
	//std::cout << "crossNumeratorA is: " << crossNumeratorA << std::endl;
	//std::cout << "crossDenominatorA is: " << crossDenominatorA << std::endl;
	float t = calculateLineSegmentIntersectScalar(crossNumeratorA, crossDenominatorA);
	std::cout << "Line Segment A scalar (variable t) is : " << t << std::endl;

	// *************** Segment B, solve for u (scalar value for Segment B)
	// following the referenced equation:  u = (q - p) x r / (r x s); note x = cross.
	// subtract point A of LineSegmentB

	float crossNumeratorB = calculate2DCross(preCrossNumerator, r);
	float crossDenominatorB = calculate2DCross(r, s);
	float u = calculateLineSegmentIntersectScalar(crossNumeratorB, crossDenominatorB);
	std::cout << "Line Segment B scalar (variable u) is : " << u << std::endl;

	/*
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
	*/

	float rsCross = calculate2DCross(r, s);
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
		analyzedResult.intersectType = LineSegmentIntersectType::NONCOLINEAR_INTERSECT;
		analyzedResult.intersectedPoint = lineSegmentA.a + (r * t);
		std::cout << "Lines intersect! " << std::endl;
		std::cout << "!!! Intersection point is: " << analyzedResult.intersectedPoint.x << ", " << analyzedResult.intersectedPoint.y << std::endl;
	}
	else
	{
		// NO_INTERSECT
		std::cout << "Lines don't intersect! " << std::endl;
	}

	
}

float LineSegmentIntersectAnalyzer::calculateLineSegmentIntersectScalar(float in_numerator, float in_denominator)
{
	float scalarValue = 0.0f;

	// check for 0 division safety
	if
	(		
		(in_numerator != 0.0f)
		&&
		(in_denominator != 0.0f)
	)
	{
		scalarValue = in_numerator / in_denominator;
	}
	return scalarValue;
}

float LineSegmentIntersectAnalyzer::calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB)
{
	float crossValue;
	crossValue = (in_crossPointA.x * in_crossPointB.y) - (in_crossPointA.y * in_crossPointB.x);
	return crossValue;
}

float LineSegmentIntersectAnalyzer::calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB)
{
	return (in_dotPointA.x * in_dotPointB.x) + (in_dotPointA.y * in_dotPointB.y);
}

bool LineSegmentIntersectAnalyzer::checkForColinearOverlap()
{
	bool returnValue = false;
	float t0 = calculate2DDot((lineSegmentB.a - lineSegmentA.a), r) / calculate2DDot(r, r);
	float t1 = calculate2DDot((lineSegmentB.a + s - lineSegmentA.a), r) / calculate2DDot(r, r);
	float rsDot = calculate2DDot(s, r);

	std::cout << "Dot of s and r is: " << rsDot << std::endl;
	std::cout << "#### t0 is: " << t0 << std::endl;
	std::cout << "#### t1 is: " << t1 << std::endl;

	if (rsDot > 0)	// lines go in same direction
	{
		if
		(
			(t0 <= 1)
			&&
			(t0 >= 0)
		)
		{
			returnValue = true;	// overlaps
		}
	}
	else if (rsDot < 0)	// lines go in opposite directions
	{
		if
		(
			(t1 <= 1)
			&&
			(t1 >= 0)
		)
		{
			returnValue = true;	// overlaps
		}
	}
	return returnValue;
}