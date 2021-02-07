#include "stdafx.h"
#include "TwoDLineSegmentIntersectAnalyzerV2.h"

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
	QuatRotationPoints points;
	glm::vec3 lineAPointA = convert2DPointTo3D(twoDLineSegmentA.a);
	glm::vec3 lineAPointB = convert2DPointTo3D(twoDLineSegmentA.b);
	glm::vec3 lineBPointA = convert2DPointTo3D(twoDLineSegmentB.a);
	glm::vec3 lineBPointB = convert2DPointTo3D(twoDLineSegmentB.b);
	points.pointsRefVector.push_back(&lineAPointA);
	points.pointsRefVector.push_back(&lineAPointB);
	points.pointsRefVector.push_back(&lineBPointA);
	points.pointsRefVector.push_back(&lineBPointB);
	QMBoolAreLinesColinear linearTester;
	bool areLinesColinear = linearTester.solve(&points, PolyDebugLevel::NONE);
	if (areLinesColinear == false)
	{
		twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) Lines NOT detected as colinear; continuing this branch. ", "\n");
		QuatRotationPoints intersectionTestPoints;
		glm::vec3 testLineAPointA = convert2DPointTo3D(twoDLineSegmentA.a);
		glm::vec3 testLineAPointB = convert2DPointTo3D(twoDLineSegmentA.b);
		glm::vec3 testLineBPointA = convert2DPointTo3D(twoDLineSegmentB.a);
		glm::vec3 testLineBPointB = convert2DPointTo3D(twoDLineSegmentB.b);
		glm::vec3 resultingIntersection;
		intersectionTestPoints.pointsRefVector.push_back(&testLineAPointA);
		intersectionTestPoints.pointsRefVector.push_back(&testLineAPointB);
		intersectionTestPoints.pointsRefVector.push_back(&testLineBPointA);
		intersectionTestPoints.pointsRefVector.push_back(&testLineBPointB);
		intersectionTestPoints.pointsRefVector.push_back(&resultingIntersection);
		QMBoolDoLinesIntersect intersectionTester;
		bool areLinesInteresecting = intersectionTester.solve(&intersectionTestPoints, PolyDebugLevel::NONE);
		if (areLinesInteresecting == true)
		{
			glm::vec3 foundIntersectionValue = intersectionTestPoints.getPointByIndex(4);
			//std::cout << "!! Point found as a result of intersection machine is: " << foundIntersectionValue.x << ", " << foundIntersectionValue.y << std::endl;
			twoDLineSegmentIntersectV2Logger.log("(TwoDLineSegmentIntersectAnalyzerV2) NONCOLINEAR_INTERSECT detected. Value is: ", foundIntersectionValue.x, ", ", foundIntersectionValue.y, "\n");
			analyzedResult.intersectedPoint.x = foundIntersectionValue.x;
			analyzedResult.intersectedPoint.y = foundIntersectionValue.y;
			analyzedResult.intersectType = TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT;
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