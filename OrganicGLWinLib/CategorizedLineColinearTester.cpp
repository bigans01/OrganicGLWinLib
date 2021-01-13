#include "stdafx.h"
#include "CategorizedLineColinearTester.h"

void CategorizedLineColinearTester::runColinearTests()
{
	// load the STriangle's line's first points
	for (int x = 0; x < 3; x++)
	{
		coplanarPoints.pointsRefVector.push_back(&hostSTriangle.triangleLines[x].pointA);
		//coplanarPoints.pointsRefVector.push_back(&hostSTriangle.triangleLines[x].pointB);
	}

	// load the candidate line's points into the coplanar points
	coplanarPoints.pointsRefVector.push_back(&candidateLine.line.pointA);
	coplanarPoints.pointsRefVector.push_back(&candidateLine.line.pointB);

	// translate by the first point of the first line in the host STriangle
	pointTranslator.performCheck(hostSTriangle.triangleLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		//std::cout << "!! prime point 0 requires translation!! " << std::endl;
		//std::cout << "It's value is: " << trackedSPolyRef->borderLines[0].pointA.x << ", " << trackedSPolyRef->borderLines[0].pointA.y << ", " << trackedSPolyRef->borderLines[0].pointA.z << std::endl;
		coplanarPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// 1.3 rotate points by the quaternion, to get to Z = 0, then round
	rotationManager.initializeAndRunForZFracture(&coplanarPoints);
	//coplanarPoints.roundAllPointsToHundredths();

	//std::cout << "!!! Printing test points. " << std::endl;
	//coplanarPoints.printPoints();
	//std::cout << "++++++++++++++++++++++++++++++ done printing test points. " << std::endl;

	// load the array of TwoDlineSegments from the STriangle appropriately
	TwoDLineSegment segmentArray[3];
	for (int x = 0; x < 3; x++)
	{
		if (x != 2)
		{
			TwoDPoint pointA = OrganicGLWinUtils::convertGlmVec3To2D(hostSTriangle.triangleLines[x].pointA);
			TwoDPoint pointB = OrganicGLWinUtils::convertGlmVec3To2D(hostSTriangle.triangleLines[x+1].pointA);
			TwoDLineSegment newSegment(pointA, pointB);
			segmentArray[x] = newSegment;
		}
		else if (x == 2)
		{
			TwoDPoint pointA = OrganicGLWinUtils::convertGlmVec3To2D(hostSTriangle.triangleLines[2].pointA);
			TwoDPoint pointB = OrganicGLWinUtils::convertGlmVec3To2D(hostSTriangle.triangleLines[0].pointA);
			TwoDLineSegment newSegment(pointA, pointB);
			segmentArray[x] = newSegment;
		}
	}

	// create the segment from the categorized line
	TwoDPoint categorizedLinePointA = OrganicGLWinUtils::convertGlmVec3To2D(candidateLine.line.pointA);
	TwoDPoint categorizedLinePointB = OrganicGLWinUtils::convertGlmVec3To2D(candidateLine.line.pointB);
	TwoDLineSegment categorizedLineSegment(categorizedLinePointA, categorizedLinePointB);

	// now, compare the categorizedLineSegment against each segment of the STriangle
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "|||||||||||||| (CoLinear Testing) Categorized Line Segment points are: point A " << categorizedLineSegment.a.x << ", " << categorizedLineSegment.a.y
		//														   << " | point B " << categorizedLineSegment.b.x << ", " << categorizedLineSegment.b.y << std::endl;
		//
		//std::cout << "|||||||||||||| (CoLinear Testing) array segment points are: point A " << segmentArray[x].a.x << ", " << segmentArray[x].a.y
		//	<< " | point B " << segmentArray[x].b.x << ", " << segmentArray[x].b.y << std::endl;

		colinearTesterLogger.log("|||||||||||||| (CoLinear Testing) Categorized Line Segment points are: point A ", categorizedLineSegment.a.x, ", ", categorizedLineSegment.a.y, ", ",
			" | point B", categorizedLineSegment.b.x, ", ", categorizedLineSegment.b.y, "\n");
		colinearTesterLogger.log("|||||||||||||| (CoLinear Testing) array segment points are: point A ", segmentArray[x].a.x, ", ", segmentArray[x].a.y,
			" | point B ", segmentArray[x].b.x, ", ", segmentArray[x].b.y, "\n");



		TwoDLineSegmentIntersectAnalyzer comparator(categorizedLineSegment, segmentArray[x], IntersectAnalyzerOption::ROUND_CROSS, colinearTesterLogLevel);
		if 
		(
			(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::COLINEAR_OVERLAP)
			&&
			(hostSTriangle.triangleLines[x].isBorderLine == 1)	// we only care about COLINEAR_OVERLAPs if the line we are analyzing is a border line.
		)
		{

			//std::cout << "!!! Warning: COLINEAR_OVERLAP detected! " << std::endl;
			colinearTesterLogger.log("!!! Warning: COLINEAR_OVERLAP detected! ", "\n");
			colinearToBorderLineDetected = true;
		}
	}

	/*
	std::cout << "!!!!!!!!!!!!!!!!!!! ------------> analysis finished. " << std::endl;

	std::cout << ":::::::::::: running hard-coded segment test. " << std::endl;
	glm::vec3 hardPointA = candidateLine.line.pointA;
	hardPointA.y += .001f;
	std::cout << "Hard point A is now: " << hardPointA.x << ", " << hardPointA.y << "," << hardPointA.z << std::endl;
	TwoDPoint convertedHardPointA = OrganicGLWinUtils::convertGlmVec3To2D(hardPointA);
	TwoDPoint convertedHardPointB = OrganicGLWinUtils::convertGlmVec3To2D(candidateLine.line.pointB);
	TwoDLineSegment hardSegment(convertedHardPointA, convertedHardPointB);

	std::cout << "!!! Hard segment stats are: " << std::endl;
	std::cout << "-> point A" << hardSegment.a.x << ", " << hardSegment.a.y
		<< " | point B " << hardSegment.b.x << ", " << hardSegment.b.y << std::endl;

	std::cout << "!!! Compared to segment stats are: " << segmentArray[1].a.x << ", " << segmentArray[1].a.y
		<< " | point B " << segmentArray[1].b.x << ", " << segmentArray[1].b.y << std::endl;


	TwoDLineSegmentIntersectAnalyzer hardCompare(hardSegment, segmentArray[1], IntersectAnalyzerOption::ROUND_CROSS);
	std::cout << ":::::::::::: end hard-coded segment test. " << std::endl;
	*/

	//int someVal = 3;
	//std::cin >> someVal;


}