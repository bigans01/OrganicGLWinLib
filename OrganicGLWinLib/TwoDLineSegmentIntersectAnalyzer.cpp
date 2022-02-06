#include "stdafx.h"
#include "TwoDLineSegmentIntersectAnalyzer.h"

void TwoDLineSegmentIntersectAnalyzer::performAnalysis()
{
	// see this URL: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	r = TwoDLineSegmentA.b - TwoDLineSegmentA.a;		// get the 2D slope for segment A, r
	s = TwoDLineSegmentB.b - TwoDLineSegmentB.a;		// get the 2D slope for segment B, s


	// first part of numerator, used in both the t and u solving equations
	TwoDPoint preCrossNumerator;
	preCrossNumerator = TwoDLineSegmentB.a;
	preCrossNumerator -= TwoDLineSegmentA.a;
	//std::cout << "PrecrossNumerator is: " << preCrossNumerator.x << ", " << preCrossNumerator.y << std::endl;
	//std::cout << "r is: " << r.x << ", " << r.y << std::endl;
	//std::cout << "s is: " << s.x << ", " << s.y << std::endl;
	twoDLineSegmentIntersectLogger.log("PrecrossNumerator is: ", preCrossNumerator.x, ", ", preCrossNumerator.y, "\n");
	twoDLineSegmentIntersectLogger.log("r is: ", r.x, ", ", r.y, "\n");
	twoDLineSegmentIntersectLogger.log("s is: ", s.x, ", ", s.y, "\n");

	// *************** Segment A, solve for t (scalar value for Segment A)
	// subtract point A of TwoDLineSegment A, from point A of TwoDLineSegment B; get the cross
	
	float crossNumeratorA = calculate2DCross(preCrossNumerator, s);
	float crossDenominatorA = calculate2DCross(r, s);
	//std::cout << "crossNumeratorA is: " << crossNumeratorA << std::endl;
	//std::cout << "crossDenominatorA is: " << crossDenominatorA << std::endl;
	float t = calculateTwoDLineSegmentIntersectScalar(crossNumeratorA, crossDenominatorA);
	//std::cout << "Line Segment A scalar (variable t) is : " << t << std::endl;
	twoDLineSegmentIntersectLogger.log("Line Segment A scalar (variable t) is : ", t, "\n");

	// *************** Segment B, solve for u (scalar value for Segment B)
	// following the referenced equation:  u = (q - p) x r / (r x s); note x = cross.
	// subtract point A of TwoDLineSegmentB

	float crossNumeratorB = calculate2DCross(preCrossNumerator, r);
	float crossDenominatorB = calculate2DCross(r, s);
	float u = calculateTwoDLineSegmentIntersectScalar(crossNumeratorB, crossDenominatorB);
	//std::cout << "Line Segment B scalar (variable u) is : " << u << std::endl;
	twoDLineSegmentIntersectLogger.log("Line Segment B scalar (variable u) is : ", u, "\n");

	/*
	// test the point values 
	// Segment A
	TwoDPoint segmentAStart = TwoDLineSegmentA.a;
	TwoDPoint segmentASlopeScaled = (r * t);
	TwoDPoint intersectedCalcPointA = segmentAStart;
	intersectedCalcPointA += segmentASlopeScaled;
	std::cout << "Segment A, intersected point: " << intersectedCalcPointA.x << ", " << intersectedCalcPointA.y << std::endl;

	// Segment B
	TwoDPoint segmentBStart = TwoDLineSegmentB.a;
	TwoDPoint segmentBSlopeScaled = (s * u);
	TwoDPoint intersectedCalcPointB = segmentBStart;
	intersectedCalcPointB += segmentBSlopeScaled;
	std::cout << "Segment B, intersected point: " << intersectedCalcPointB.x << ", " << intersectedCalcPointB.y << std::endl;
	*/

	float rsCross = calculate2DCross(r, s);
	if (analyzerOption == IntersectAnalyzerOption::ROUND_CROSS)
	{

		//std::cout << "!! Rounding cross, as requested by option. " << std::endl;
		twoDLineSegmentIntersectLogger.log("!! Rounding cross, as requested by option. ", "\n");
		
		int threeVal = 3;

		rsCross = float(floor(rsCross * 1000 + 0.5) / 1000);
	}

	//std::cout << "Value of rsCross: " << rsCross << std::endl;
	//std::cout << "(pre-calc) Line Segment A scalar (variable t) is : " << t << std::endl;
	//std::cout << "(pre-calc) Line Segment B scalar (variable u) is : " << u << std::endl;

	twoDLineSegmentIntersectLogger.log("Value of rsCross: ", rsCross, "\n");
	twoDLineSegmentIntersectLogger.log("(pre-calc) Line Segment A scalar (variable t) is : ", t, "\n");
	twoDLineSegmentIntersectLogger.log("(pre-calc) Line Segment B scalar (variable u) is : ", u, "\n");


	if (t == 1.0f)
	{
		//std::cout << "::: NOTE: value of t is 1.0f! " << std::endl;
		twoDLineSegmentIntersectLogger.log("::: NOTE: value of t is 1.0f! ", "\n");
	}
	if (u == 1.0f)
	{
		//std::cout << "::: NOTE: value of u is 1.0f! " << std::endl;
		twoDLineSegmentIntersectLogger.log("::: NOTE: value of u is 1.0f! ", "\n");
	}

	//if (rsCross == 0.0f)	
	if (isFloatWithinColinearThreshold(rsCross) == true)
	{
		// COLINEAR conditions
		//if (calculate2DCrossWithOptionCheck((TwoDLineSegmentB.a - TwoDLineSegmentA.a), r) == 0.0f)
		if (isFloatWithinColinearThreshold(calculate2DCrossWithOptionCheck((TwoDLineSegmentB.a - TwoDLineSegmentA.a), r)) == true)
		{
			if (checkForColinearOverlap() == true)		// COLINEAR_OVERLAP
			{
				//std::cout << "Lines are colinear overlap! " << std::endl;
				twoDLineSegmentIntersectLogger.log("Lines are colinear overlap! ", "\n");
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::COLINEAR_OVERLAP;
			}
			else   // COLINEAR_NOOVERLAP
			{
				//std::cout << "Lines are colinear, and don't overlap! " << std::endl;
				twoDLineSegmentIntersectLogger.log("Lines are colinear, and don't overlap! ", "\n");
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::COLINEAR_NOOVERLAP;
			}
			
		}

		// PARALLEL, NON-INTERSECTING
		//else if (calculate2DCrossWithOptionCheck((TwoDLineSegmentB.a - TwoDLineSegmentA.a), r) != 0.0f)	// PARALLEL
		if (isFloatWithinColinearThreshold(calculate2DCrossWithOptionCheck((TwoDLineSegmentB.a - TwoDLineSegmentA.a), r)) == false)
		{
			//std::cout << "Lines are parallel, but don't intersect! " << std::endl;
			twoDLineSegmentIntersectLogger.log("Lines are parallel, but don't intersect!", "\n");
		}
		
	}
	else if 
	(
		//(rsCross != 0.0f)
		(isFloatWithinColinearThreshold(rsCross) == false)
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
		analyzedResult.intersectedPoint = round2DPointToHundredths(TwoDLineSegmentA.a + (r * t));
		//analyzedResult.intersectType = TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT;

		if
			(

				!
				(
				(analyzedResult.intersectedPoint.x == TwoDLineSegmentB.a.x)
					&&
					(analyzedResult.intersectedPoint.y == TwoDLineSegmentB.a.y)
					)

				&&

				!
				(
				(analyzedResult.intersectedPoint.x == TwoDLineSegmentB.b.x)
					&&
					(analyzedResult.intersectedPoint.y == TwoDLineSegmentB.b.y)
					)
				)

		{
			analyzedResult.intersectType = TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT;
			//std::cout << "!! Intersected point doesn't match one of the endpoints of the line it intersects; intersect is therefore VALID. " << std::endl;
			twoDLineSegmentIntersectLogger.log("!! Intersected point doesn't match one of the endpoints of the line it intersects; intersect is therefore VALID. ", "\n");
		}
		else
		{
			analyzedResult.intersectType = TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECTS_POINT_PRECISE;
			//std::cout << "(2) !!! Warning, analyzedResult.intersectedPoint matched one of the segment it intersected! Should therefore be NONCOLINEAR_INTERSECTS_POINT_PRECISE" << std::endl;
			twoDLineSegmentIntersectLogger.log("(2) !!! Warning, analyzedResult.intersectedPoint matched one of the segment it intersected! Should therefore be NONCOLINEAR_INTERSECTS_POINT_PRECISE", "\n");
		}


		//std::cout << "Lines intersect! " << std::endl;
		//std::cout << "!!! Intersection point is: " << analyzedResult.intersectedPoint.x << ", " << analyzedResult.intersectedPoint.y << std::endl;
		twoDLineSegmentIntersectLogger.log("Lines intersect! ", "\n");
		twoDLineSegmentIntersectLogger.log("!!! Intersection point is: ", analyzedResult.intersectedPoint.x, ", ", analyzedResult.intersectedPoint.y, "\n");

		/*
		if
		(
			(analyzedResult.intersectedPoint.x == TwoDLineSegmentB.a.x)
			&&
			(analyzedResult.intersectedPoint.y == TwoDLineSegmentB.a.y)
		)
		{
			std::cout << "! Warning, intersectedPoint is same as one of the two points on the line it intersects! " << std::endl;
		}
		*/

		//std::cout << "Points of intersected line are: " << TwoDLineSegmentB.a.x << ", " << TwoDLineSegmentB.a.y << " | " << TwoDLineSegmentB.b.x << ", " << TwoDLineSegmentB.b.y << std::endl;
		twoDLineSegmentIntersectLogger.log("Points of intersected line are: ", TwoDLineSegmentB.a.x, ", ", TwoDLineSegmentB.a.y, " | ", TwoDLineSegmentB.b.x, ", ", TwoDLineSegmentB.b.y, "\n");

	}
	else
	{
		// NO_INTERSECT
		//std::cout << "Lines don't intersect! " << std::endl;
		twoDLineSegmentIntersectLogger.log("Lines don't intersect! ", "\n");
	}

	
}

bool TwoDLineSegmentIntersectAnalyzer::isFloatWithinColinearThreshold(float in_threshold)
{
	bool isWithin = false;
	if
	(
		//(in_threshold >= -.001f)
		//&&
		//(in_threshold <= .001f)

		
		//(in_threshold >= -.007f)
		//&&
		//(in_threshold <= .007f)
		
		(in_threshold >= -.020f)
		&&
		(in_threshold <= .020f)
	)
	{
		//std::cout << "!! Float is WITHIN threshold! " << std::endl;
		twoDLineSegmentIntersectLogger.log("!! Float is WITHIN threshold! ", "\n");
		isWithin = true;
	}
	return isWithin;
}

float TwoDLineSegmentIntersectAnalyzer::calculateTwoDLineSegmentIntersectScalar(float in_numerator, float in_denominator)
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

float TwoDLineSegmentIntersectAnalyzer::calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB)
{
	float crossValue;
	crossValue = (in_crossPointA.x * in_crossPointB.y) - (in_crossPointA.y * in_crossPointB.x);
	//std::cout << "!! Cross value is: " << crossValue << std::endl;
	twoDLineSegmentIntersectLogger.log("!! Cross value is: ", crossValue, "\n");
	return crossValue;
}

float TwoDLineSegmentIntersectAnalyzer::calculate2DCrossWithOptionCheck(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB)
{
	float crossValue;
	crossValue = (in_crossPointA.x * in_crossPointB.y) - (in_crossPointA.y * in_crossPointB.x);
	if (analyzerOption == IntersectAnalyzerOption::ROUND_CROSS)
	{
		crossValue = float(floor(crossValue * 1000 + 0.5) / 1000);
		//std::cout << "! (Optioned) Cross value is (rounded to nearest hundredths) is : " << crossValue << std::endl;
		twoDLineSegmentIntersectLogger.log("! (Optioned) Cross value is (rounded to nearest hundredths) is : ", crossValue, "\n");
	}
	return crossValue;

}

float TwoDLineSegmentIntersectAnalyzer::calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB)
{
	return (in_dotPointA.x * in_dotPointB.x) + (in_dotPointA.y * in_dotPointB.y);
}

float TwoDLineSegmentIntersectAnalyzer::calculate2DDotWithOptionCheck(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB)
{
	float returnDot = (in_dotPointA.x * in_dotPointB.x) + (in_dotPointA.y * in_dotPointB.y);
	if (analyzerOption == IntersectAnalyzerOption::ROUND_CROSS)
	{
		returnDot = float(floor(returnDot * 1000 + 0.5) / 1000);
		//std::cout << "! (Optioned) dot product is (rounded to nearest thousandths) is: " << returnDot << std::endl;
		twoDLineSegmentIntersectLogger.log("! (Optioned) dot product is (rounded to nearest thousandths) is: ", returnDot);
	}
	return returnDot;

}

bool TwoDLineSegmentIntersectAnalyzer::checkForColinearOverlap()
{
	bool returnValue = false;
	float t0 = calculate2DDotWithOptionCheck((TwoDLineSegmentB.a - TwoDLineSegmentA.a), r) / calculate2DDot(r, r);
	float t1 = calculate2DDotWithOptionCheck((TwoDLineSegmentB.a + s - TwoDLineSegmentA.a), r) / calculate2DDot(r, r);
	float rsDot = calculate2DDotWithOptionCheck(s, r);

	//std::cout << "!!!! Line segment A, point A is: " << TwoDLineSegmentA.a.x << ", " << TwoDLineSegmentA.a.y << std::endl;
	//std::cout << "Dot of s and r is: " << rsDot << std::endl;
	//std::cout << "#### t0 is: " << t0 << std::endl;
	//std::cout << "#### t1 is: " << t1 << std::endl;
	twoDLineSegmentIntersectLogger.log("!!!! Line segment A, point A is: ", TwoDLineSegmentA.a.x, ", ", TwoDLineSegmentA.a.y, "\n");
	twoDLineSegmentIntersectLogger.log("Dot of s and r is: ", rsDot, "\n");
	twoDLineSegmentIntersectLogger.log("#### t0 is: ", t0, "\n");
	twoDLineSegmentIntersectLogger.log("#### t1 is: ", t1, "\n");


	if (rsDot > 0)	// lines go in same direction
	{
		if
		(
			(t0 <= 1)
			&&
			//(t0 >= 0)
			(t1 >= 0)
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
			//(t1 >= 0)
			(t0 >= 0)

			// the line(s) should be aligned to the x axis; so check
			//(TwoDLineSegmentB.a.x <= TwoDLineSegmentA.a.x)
			//&&
			//(TwoDLineSegmentB.b.x >= TwoDLineSegmentA.a.x)
		)
		{
			returnValue = true;	// overlaps
		}
	}
	return returnValue;
}

TwoDPoint TwoDLineSegmentIntersectAnalyzer::round2DPointToHundredths(TwoDPoint in_point)
{
	TwoDPoint returnPoint = in_point;
	returnPoint.x = float(floor(in_point.x * 100 + 0.5) / 100);
	returnPoint.y = float(floor(in_point.y * 100 + 0.5) / 100);
	return returnPoint;
}