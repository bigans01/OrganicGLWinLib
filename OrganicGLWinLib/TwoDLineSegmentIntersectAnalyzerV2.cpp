#include "stdafx.h"
#include "TwoDLineSegmentIntersectAnalyzerV2.h"

void TwoDLineSegmentIntersectAnalyzerV2::setColinearQMDebugLevel(PolyDebugLevel in_debugLevel)
{
	colinearQMDebugLevel = in_debugLevel;
}
void TwoDLineSegmentIntersectAnalyzerV2::setIntersectionQMDebugLevel(PolyDebugLevel in_debugLevel)
{
	intersectingQMDebugLevel = in_debugLevel;
}

void TwoDLineSegmentIntersectAnalyzerV2::performAnalysis()
{
	// see this URL: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	r = twoDLineSegmentA.b - twoDLineSegmentA.a;		// get the 2D slope for segment A, r
	s = twoDLineSegmentB.b - twoDLineSegmentB.a;		// get the 2D slope for segment B, s


	// first part of numerator, used in both the t and u solving equations
	TwoDPoint preCrossNumerator;
	preCrossNumerator = twoDLineSegmentB.a;
	preCrossNumerator -= twoDLineSegmentA.a;
	//std::cout << "PrecrossNumerator is: " << preCrossNumerator.x << ", " << preCrossNumerator.y << std::endl;
	//std::cout << "r is: " << r.x << ", " << r.y << std::endl;
	//std::cout << "s is: " << s.x << ", " << s.y << std::endl;
	int someVal = 3;
	twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) PrecrossNumerator is: ", preCrossNumerator.x, ", ", preCrossNumerator.y, "\n");
	twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) r is: ", r.x, ", ", r.y, "\n");
	twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) s is: ", s.x, ", ", s.y, "\n");

	// *************** Segment A, solve for t (scalar value for Segment A)
	// subtract point A of TwoDLineSegment A, from point A of TwoDLineSegment B; get the cross

	float crossNumeratorA = calculate2DCross(preCrossNumerator, s);
	float crossDenominatorA = calculate2DCross(r, s);
	//std::cout << "crossNumeratorA is: " << crossNumeratorA << std::endl;
	//std::cout << "crossDenominatorA is: " << crossDenominatorA << std::endl;
	float t = calculateTwoDLineSegmentIntersectScalar(crossNumeratorA, crossDenominatorA);
	//std::cout << "Line Segment A scalar (variable t) is : " << t << std::endl;
	twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Line Segment A scalar (variable t) is : ", t, "\n");

	// *************** Segment B, solve for u (scalar value for Segment B)
	// following the referenced equation:  u = (q - p) x r / (r x s); note x = cross.
	// subtract point A of TwoDLineSegmentB

	float crossNumeratorB = calculate2DCross(preCrossNumerator, r);
	float crossDenominatorB = calculate2DCross(r, s);
	float u = calculateTwoDLineSegmentIntersectScalar(crossNumeratorB, crossDenominatorB);
	//std::cout << "Line Segment B scalar (variable u) is : " << u << std::endl;
	twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Line Segment B scalar (variable u) is : ", u, "\n");

	// perform colinear test; need to convert the 2d points to 3d before putting them into the QM.
	glm::vec3 lineAPointA = convert2DPointTo3D(twoDLineSegmentA.a);
	glm::vec3 lineAPointB = convert2DPointTo3D(twoDLineSegmentA.b);
	glm::vec3 lineBPointA = convert2DPointTo3D(twoDLineSegmentB.a);
	glm::vec3 lineBPointB = convert2DPointTo3D(twoDLineSegmentB.b);
	bool areLinesColinear = QuatUtils::checkIfLinesAreColinear(lineAPointA, lineAPointB, lineBPointA, lineBPointB, colinearQMDebugLevel);
	if (areLinesColinear == false)
	{
		twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Lines NOT detected as colinear; continuing this branch. ", "\n");
		glm::vec3 testLineAPointA = convert2DPointTo3D(twoDLineSegmentA.a);
		glm::vec3 testLineAPointB = convert2DPointTo3D(twoDLineSegmentA.b);
		glm::vec3 testLineBPointA = convert2DPointTo3D(twoDLineSegmentB.a);
		glm::vec3 testLineBPointB = convert2DPointTo3D(twoDLineSegmentB.b);
		glm::vec3 resultingIntersection;
		bool areLinesInteresecting = QuatUtils::areLinesIntersecting(&testLineAPointA, &testLineAPointB, &testLineBPointA, &testLineBPointB, &resultingIntersection, intersectingQMDebugLevel);
		if (areLinesInteresecting == true)
		{
			glm::vec3 foundIntersectionValue = resultingIntersection;
			//std::cout << "!! Point found as a result of intersection machine is: " << foundIntersectionValue.x << ", " << foundIntersectionValue.y << std::endl;
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) NONCOLINEAR_INTERSECT detected. Value is: ", foundIntersectionValue.x, ", ", foundIntersectionValue.y, "\n");
			analyzedResult.intersectedPoint.x = foundIntersectionValue.x;
			analyzedResult.intersectedPoint.y = foundIntersectionValue.y;

			// now, determine if a T-junction exists; 
			float lineALength = glm::distance(convert2DPointTo3D(twoDLineSegmentA.a), convert2DPointTo3D(twoDLineSegmentA.b));
			float distBetweenLineAPointAAndIntersectedPoint = glm::distance(convert2DPointTo3D(twoDLineSegmentA.a), foundIntersectionValue);
			float distBetweenLineAPointBAndIntersectedPoint = glm::distance(convert2DPointTo3D(twoDLineSegmentA.b), foundIntersectionValue);

			//std::cout << "###--> length of line A: " << lineALength << std::endl;
			//std::cout << "###--> dist between pointA and intersected value: " << distBetweenLineAPointAAndIntersectedPoint << std::endl;
			//std::cout << "###--> dist between pointB and intersected value: " << distBetweenLineAPointBAndIntersectedPoint << std::endl;
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> length of line A: ", lineALength, "\n");
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> (Line A) dist between pointA and intersected value: ", distBetweenLineAPointAAndIntersectedPoint, "\n");
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> (Line A) dist between pointB and intersected value: ", distBetweenLineAPointBAndIntersectedPoint, "\n");

			bool tJunctionTestForLineAPointAToIntersectedPoint = runTJunctionTest(lineALength, distBetweenLineAPointAAndIntersectedPoint);
			bool tJunctionTestForLineAPointBToIntersectedPoint = runTJunctionTest(lineALength, distBetweenLineAPointBAndIntersectedPoint);

			float lineBLength = glm::distance(convert2DPointTo3D(twoDLineSegmentB.a), convert2DPointTo3D(twoDLineSegmentB.b));
			float distBetweenLineBPointAAndIntersectedPoint = glm::distance(convert2DPointTo3D(twoDLineSegmentB.a), foundIntersectionValue);
			float distBetweenLineBPointBAndIntersectedPoint = glm::distance(convert2DPointTo3D(twoDLineSegmentB.b), foundIntersectionValue);

			//std::cout << "###--> length of line B: " << lineBLength << std::endl;
			//std::cout << "###--> dist between pointA and intersected value: " << distBetweenLineBPointAAndIntersectedPoint << std::endl;
			//std::cout << "###--> dist between pointB and intersected value: " << distBetweenLineBPointBAndIntersectedPoint << std::endl;

			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> length of line B: ", lineALength, "\n");
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> (Line B) dist between pointA and intersected value: ", distBetweenLineBPointAAndIntersectedPoint, "\n");
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) ###--> (Line B) dist between pointB and intersected value: ", distBetweenLineBPointBAndIntersectedPoint, "\n");

			bool tJunctionTestForLineBPointAToIntersectedPoint = runTJunctionTest(lineBLength, distBetweenLineBPointAAndIntersectedPoint);
			bool tJunctionTestForLineBPointBToIntersectedPoint = runTJunctionTest(lineBLength, distBetweenLineBPointBAndIntersectedPoint);

			if (tJunctionTestForLineAPointAToIntersectedPoint == true)
			{
				//std::cout << "!!! Notice, T-junction detected: Point B of Line A splits Line B." << std::endl;
				//std::cout << "!!! Resulting t-junction type is: T_JUNCTION_A_SPLITS_B_VIA_POINT_B" << std::endl;
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Notice, T-junction detected: Point B of Line A splits Line B.", "\n");
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Resulting t-junction type is: T_JUNCTION_A_SPLITS_B_VIA_POINT_B", "\n");

				analyzedResult.intersectType = TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_B;
			}
			else if (tJunctionTestForLineAPointBToIntersectedPoint == true)
			{
				//std::cout << "!!! Notice, T-junction detected, Point A of Line A splits Line B. " << std::endl;
				//std::cout << "!!! Resulting t-junction type is: T_JUNCTION_A_SPLITS_B_VIA_POINT_A" << std::endl;
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Notice, T-junction detected, Point A of Line A splits Line B.", "\n");
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Resulting t-junction type is: T_JUNCTION_A_SPLITS_B_VIA_POINT_A", "\n");
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_A;
			}
			else if (tJunctionTestForLineBPointAToIntersectedPoint == true)
			{
				//std::cout << "!!! Notice, T-junction detected, Point B of Line B splits Line A. " << std::endl;
				//std::cout << "!!! Resulting t-junction type is: T_JUNCTION_B_SPLITS_A_VIA_POINT_B" << std::endl;
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Notice, T-junction detected, Point B of Line B splits Line A.", "\n");
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Resulting t-junction type is: T_JUNCTION_B_SPLITS_A_VIA_POINT_B", "\n");
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::T_JUNCTION_B_SPLITS_A_VIA_POINT_B;
			}
			else if (tJunctionTestForLineBPointBToIntersectedPoint == true)
			{
				//std::cout << "!!! Notice, T-junction detected, Point A of Line B splits Line A. " << std::endl;
				//std::cout << "!!! Resulting t-junction type is: T_JUNCTION_B_SPLITS_A_VIA_POINT_A" << std::endl;
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Notice, T-junction detected, Point A of Line B splits Line A.", "\n");
				twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) !!! Resulting t-junction type is: T_JUNCTION_B_SPLITS_A_VIA_POINT_A", "\n");
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::T_JUNCTION_B_SPLITS_A_VIA_POINT_A;
			}
			else   // as long as no T-junction detected, return as being a NONCOLINEAR_INTERSECT.
			{
				analyzedResult.intersectType = TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT;
			}
		}
		else
		{
			//std::cout << "!! lines do NOT intersect. " << std::endl;
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) NO_INTERSECT detected.", "\n");
			analyzedResult.intersectType = TwoDLineSegmentIntersectType::NO_INTERSECT;
		}

		//analyzedResult.intersectedPoint = twoDLineSegmentA.a + (r * t);


	}
	else if (areLinesColinear == true)
	{
		twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Lines detected as colinear. ", "\n");
		//std::cout << "!!! Notice, colinear line detected: " << std::endl;
		//std::cout << "!!! Line A: " << lineAPointA.x << ", " << lineAPointA.y << std::endl;
		//std::cout << "!!! Line B: " << lineBPointA.x << ", " << lineBPointB.y << std::endl;
		twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Colinear Line A | point A: ", lineAPointA.x, ", ", lineAPointA.y, " | point B: ", lineAPointB.x, ", ", lineAPointB.y, "\n");
		twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Colinear Line B | point A: ", lineBPointA.x, ", ", lineBPointA.y, " | point B: ", lineBPointB.x, ", ", lineBPointB.y, "\n");
		//int colinearTempStop = 3;
		//std::cin >> colinearTempStop;
		twoDLineSegmentIntersectV2Logger.waitForDebugInput();
	}
}

glm::vec3 TwoDLineSegmentIntersectAnalyzerV2::convert2DPointTo3D(TwoDPoint in_2dPoint)
{
	glm::vec3 return3dPoint;
	return3dPoint.x = in_2dPoint.x;
	return3dPoint.y = in_2dPoint.y;
	return return3dPoint;
}

float TwoDLineSegmentIntersectAnalyzerV2::calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB)
{
	float crossValue;
	crossValue = (in_crossPointA.x * in_crossPointB.y) - (in_crossPointA.y * in_crossPointB.x);
	//std::cout << "!! Cross value is: " << crossValue << std::endl;
	twoDLineSegmentIntersectV2Logger.log("!! Cross value is: ", crossValue, "\n");
	return crossValue;
}

float TwoDLineSegmentIntersectAnalyzerV2::calculateTwoDLineSegmentIntersectScalar(float in_numerator, float in_denominator)
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

bool TwoDLineSegmentIntersectAnalyzerV2::runTJunctionTest(float in_lineLength, float in_distanceToIntersectedPointToAnalyze)
{
	bool returnValue = false;
	float roundedLength = float(floor(in_lineLength * 10000 + 0.5) / 10000);
	float roundedDistanceToIntersectedPointToAnalyze = float(floor(in_distanceToIntersectedPointToAnalyze * 10000 + 0.5) / 10000);
	//float roundedDistanceFromAtoIntersectedPoint = float(floor(in_distanceFromAtoIntersectedPoint * 10000 + 0.5) / 10000);
	//float roundedDistanceFromBtoIntersectedPoint = float(floor(in_distanceFromBToIntersectedPoint * 10000 + 0.5) / 10000);
	//if
	//(
		//(roundedLength == roundedDistanceFromAtoIntersectedPoint)
		//||
		//(roundedLength == roundedDistanceFromBtoIntersectedPoint)
	//)

	/*
	if (roundedLength == roundedDistanceToIntersectedPointToAnalyze)
	{
		returnValue = true;
	}
	*/

	float percentageThresholdUpperBound = 1.0001f;
	float precentageThresholdLowerBound = .9999f;

	float resultingPercentage = in_lineLength / in_distanceToIntersectedPointToAnalyze;
	//std::cout << "TEST: resultingPercentage would be: " << resultingPercentage << std::endl;
	
	if
	(
		(resultingPercentage < percentageThresholdUpperBound)
		&&
		(resultingPercentage > precentageThresholdLowerBound)
	)
	{
		returnValue = true;
	}
	return returnValue;
}