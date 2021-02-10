#include "stdafx.h"
#include "CuttableTriangle.h"

CuttableTriangle::CuttableTriangle(STriangle in_cuttableTriangle)
{
	// find the centroid
	glm::vec3 pointA = cuttableTriangleLines[0].pointA;
	glm::vec3 pointB = cuttableTriangleLines[1].pointA;
	glm::vec3 pointC = cuttableTriangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{
		QuatRotationPoints points;	// point 0 = pointA of line
									// point 1 = pointB of line
									// point 2 = centroid
		glm::vec3 pointACopy = in_cuttableTriangle.triangleLines[x].pointA;
		glm::vec3 pointBCopy = in_cuttableTriangle.triangleLines[x].pointB;
		glm::vec3 centroidCopy = centroid;
		points.pointsRefVector.push_back(&pointACopy);
		points.pointsRefVector.push_back(&pointBCopy);
		points.pointsRefVector.push_back(&centroidCopy);

		QMVec3FindCentroidFacingNormal normalFinder;
		glm::vec3 centroidFacingNormal = normalFinder.solve(&points, PolyDebugLevel::NONE);
		CuttableTriangleLine newLine(in_cuttableTriangle.triangleLines[x].pointA, in_cuttableTriangle.triangleLines[x].pointB, centroidFacingNormal);
		cuttableTriangleLines[x] = newLine;
	}
}

void CuttableTriangle::compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	// perform consumption tests; if they fail, continue with normal operations in final branch.

	// do all points of the CuttableTriangle lie within the CuttingTriangle?
	if (testIfCuttingTriangleConsumesThisTriangle(in_cuttingTriangleRef) == true)
	{
		// we should be done if this is the case.
	}

	// otherwise, attempt to generate attempts as normal.
	else
	{

		// first, run each CuttableTriangleLine against each CuttingTriangleLine.
		for (int currentCuttableTriangleLineID = 0; currentCuttableTriangleLineID < 3; currentCuttableTriangleLineID++)
		{
			for (int currentCuttingTriangleLineID = 0; currentCuttingTriangleLineID < 3; currentCuttingTriangleLineID++)
			{

				//compareCuttableTriangleLineToCuttingTriangleLine(currentCuttableTriangleLineID, 
				//												&cuttableTriangleLines[currentCuttableTriangleLineID], 
				//												currentCuttingTriangleLineID, 
				//												&in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID]);
				TwoDLineSegment cuttableSegment(cuttableTriangleLines[currentCuttableTriangleLineID].pointA.x,
					cuttableTriangleLines[currentCuttableTriangleLineID].pointA.y,
					cuttableTriangleLines[currentCuttableTriangleLineID].pointB.x,
					cuttableTriangleLines[currentCuttableTriangleLineID].pointB.y);
				TwoDLineSegment cuttingSegment(in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.x,
					in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.y,
					in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.x,
					in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.y);

				std::cout << ":::: cuttableSegment points: A> " << cuttableSegment.a.x << ", " << cuttableSegment.a.y << " | B> " << cuttableSegment.b.x << ", " << cuttableSegment.b.y << std::endl;
				std::cout << ":::: cuttingSegment points: A> " << cuttingSegment.a.x << ", " << cuttingSegment.a.y << " | B> " << cuttingSegment.b.x << ", " << cuttingSegment.b.y << std::endl;

				TwoDLineSegmentIntersectAnalyzerV2 analyzerV2(cuttableSegment, cuttingSegment, PolyDebugLevel::NONE);
				if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
				{
					glm::vec3 convertedPoint = convert2DpointTo3D(analyzerV2.analyzedResult.intersectedPoint);
					// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
					cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, convertedPoint);

					// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
					in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, convertedPoint);

					std::cout << "!!! Non-colinear intersection detected; cuttable ID is: " << currentCuttableTriangleLineID
						<< " | cutting ID is: " << currentCuttingTriangleLineID
						<< " | point is: " << analyzerV2.analyzedResult.intersectedPoint.x << ", " << analyzerV2.analyzedResult.intersectedPoint.y << std::endl;
					int outputVal = 3;
					std::cin >> outputVal;



				}
			}
		}

		// second, check for any slicing conditions that might have been met -- this would happen when any cutting line in the cutting triangle has 2 intersections
		// registered to it. Build the slicing attempt (via an instance of TwoDCrawlingAttempt),
		// and update the cuttableIntersetionManagers, by removing records that refer to the cutting line which had 2 lines.
		buildAllSlicingAttempts(in_cuttingTriangleRef);

		// third, find the first of any other cuttingTriangleLines which have 1 intersection, if they exist.
		buildRemainingAttempts(in_cuttingTriangleRef);

		// fourth: test whether or not this triangle consumes the CuttingTriangle; this is only true when:
		// A) all points of the CuttingTriangle are within the CuttableTriangle
		// B) there are no attempts built, meaning there are 0 intersections.
		//
		// If true, do special logic. Otherwise, if false, produce cut line pools as normal.
		if (testIfThisTriangleConsumesCuttingTriangle(in_cuttingTriangleRef) == true)
		{
			// special logic.
		}
		else
		{
			// fourth, build the CutLinePools from each attempt.
			produceCutLinePoolsFromAttempts(in_cuttingTriangleRef);
		}
	}
}

bool CuttableTriangle::testIfCuttingTriangleConsumesThisTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	return false;	// placeholder
}

bool CuttableTriangle::testIfThisTriangleConsumesCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	return false;	// placeholder
}

void CuttableTriangle::buildAllSlicingAttempts(CuttingTriangle* in_cuttingTriangleRef)
{
	for (int x = 0; x < 3; x++)
	{
		if (in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.numberOfRecords() == 2)
		{
			//std::cout << "!!! SLICE detected in cutting triangle line, line ID is: " << x << std::endl;
			//int sliceVal = 3;
			//std::cin >> sliceVal;

			auto intersectionRecordsBegin = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.begin();
			auto intersectionRecordsEnd = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.rbegin();
			TwoDCrawlingAttempt slicingAttempt(TwoDCrawlingType::SLICE, x, intersectionRecordsBegin->first, intersectionRecordsBegin->second, intersectionRecordsEnd->first, intersectionRecordsEnd->second);
			crawlingAttemptsVector.push_back(slicingAttempt);

			// remove corresponding entries from the cuttable triangle lines
			cuttableTriangleLines[intersectionRecordsBegin->first].cuttableIntersectionManager.eraseRecord(x);
			cuttableTriangleLines[intersectionRecordsEnd->first].cuttableIntersectionManager.eraseRecord(x);

			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for beginIntersectionLineID: " << cuttableTriangleLines[beginIntersectionLineID->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for intersectionRecordsEnd: " << cuttableTriangleLines[intersectionRecordsEnd->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
		}
	}
}

void CuttableTriangle::buildRemainingAttempts(CuttingTriangle* in_cuttingTriangleRef)
{
	for (int x = 0; x < 3; x++)
	{
		// we found a line that had exactly 1 record; generate what we need and then break.
		if (in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.numberOfRecords() == 1)	
		{
			std::cout << "!! Found remaining attempt, building and breaking. " << std::endl;
			auto intersectionRecordsBegin = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.begin();
			TwoDCrawlingAttempt typicalAttempt(TwoDCrawlingType::TYPICAL, x, intersectionRecordsBegin->first, intersectionRecordsBegin->second);
			crawlingAttemptsVector.push_back(typicalAttempt);
			break;
		}
	}
}

void CuttableTriangle::produceCutLinePoolsFromAttempts(CuttingTriangle* in_cuttingTriangleRef)
{
	auto attemptsBegin = crawlingAttemptsVector.begin();
	auto attemptsEnd = crawlingAttemptsVector.end();
	for (; attemptsBegin != attemptsEnd; attemptsBegin++)
	{
		if (attemptsBegin->crawlingType == TwoDCrawlingType::SLICE)
		{
			std::cout << "SLICE crawl type found..." << std::endl;
			buildLinesFromSliceAttempt(*attemptsBegin, in_cuttingTriangleRef);
		}
		else if (attemptsBegin->crawlingType == TwoDCrawlingType::TYPICAL)
		{
			std::cout << "TYPICAL crawl type found..." << std::endl;
			buildLinesFromTypicalAttempt(*attemptsBegin, in_cuttingTriangleRef);
		}
	}
}

CutLinePool CuttableTriangle::buildLinesFromTypicalAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef)
{
	CutLinePool returnPool;

	// common point between both lines:
	glm::vec3 sharedPoint = in_attempt.beginIntersectingPoint;

	// First, find the cyling direction. 
	// line A point values, from the CuttingLine
	glm::vec3 lineAPointA = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointA;
	glm::vec3 lineAPointB = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointB;

	// outward facing normal
	glm::vec3 cuttingLineNormal = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].outwardFacingNormal;
	int idOfLineFoundInCuttingLine = in_attempt.beginIntersectionLineID;

	// line B point values, from the CuttableLine
	glm::vec3 lineBPointA = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointA;
	glm::vec3 lineBPointB = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointB;

	std::cout << "::> cutting line point A: " << lineAPointA.x << ", " << lineAPointA.y << ", " << lineAPointA.z << std::endl;
	std::cout << "::> cutting line point B: " << lineAPointB.x << ", " << lineAPointB.y << ", " << lineAPointB.z << std::endl;
	std::cout << "::> cuttable line point A: " << lineBPointA.x << ", " << lineBPointA.y << ", " << lineBPointA.z << std::endl;
	std::cout << "::> cuttable line point B: " << lineBPointB.x << ", " << lineBPointB.y << ", " << lineBPointB.z << std::endl;
	// two quat machines for finding the points: one for the cutting line, one for the cuttable line

	QuatRotationPoints solveForCuttableLinePoints;
	glm::vec3 solveForCuttableLineAPointA = lineBPointA;
	glm::vec3 solveForCuttableLineAPointB = lineBPointB;
	glm::vec3 solveForCuttableLineBPointA = lineAPointA;
	glm::vec3 solveForCuttableLineBPointB = lineAPointB;
	glm::vec3 solveForCuttableLineInwardNormal = cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal;
	solveForCuttableLinePoints.pointsRefVector.push_back(&solveForCuttableLineAPointA);
	solveForCuttableLinePoints.pointsRefVector.push_back(&solveForCuttableLineAPointB);
	solveForCuttableLinePoints.pointsRefVector.push_back(&solveForCuttableLineBPointA);
	solveForCuttableLinePoints.pointsRefVector.push_back(&solveForCuttableLineBPointB);
	solveForCuttableLinePoints.pointsRefVector.push_back(&solveForCuttableLineInwardNormal);
	QMVec3FindCyclingDirectionPoint cuttableLineSolver;
	glm::vec3 determinedCuttingPointToUse = cuttableLineSolver.solve(&solveForCuttableLinePoints, PolyDebugLevel::NONE);
	std::cout << "::> Cuttable line normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;
	std::cout << "::> Cutting point to use, determined by cuttable line: " << determinedCuttingPointToUse.x << ", " << determinedCuttingPointToUse.y << ", " << determinedCuttingPointToUse.z << std::endl;




	QuatRotationPoints solveForCuttingLinePoints;
	glm::vec3 solveForCuttingLineAPointA = lineAPointA;
	glm::vec3 solveForCuttingLineAPointB = lineAPointB;
	glm::vec3 solveForCuttingLineBPointA = lineBPointA;
	glm::vec3 solveForCuttingLineBPointB = lineBPointB;
	glm::vec3 solveForCuttingOutwardNormal = cuttingLineNormal;
	solveForCuttingLinePoints.pointsRefVector.push_back(&solveForCuttingLineAPointA);
	solveForCuttingLinePoints.pointsRefVector.push_back(&solveForCuttingLineAPointB);
	solveForCuttingLinePoints.pointsRefVector.push_back(&solveForCuttingLineBPointA);
	solveForCuttingLinePoints.pointsRefVector.push_back(&solveForCuttingLineBPointB);
	solveForCuttingLinePoints.pointsRefVector.push_back(&solveForCuttingOutwardNormal);
	QMVec3FindCyclingDirectionPoint cuttingLineSolver;
	glm::vec3 determinedPoint = cuttingLineSolver.solve(&solveForCuttingLinePoints, PolyDebugLevel::NONE);
	std::cout << "::> Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;
	std::cout << "::> Cuttable point to use, determined by cutting line: " << determinedPoint.x << ", " << determinedPoint.y << ", " << determinedPoint.z << std::endl;

	// the CutLine spawned from the CuttingLine should be: 
	// point A = selected end point from cuttableLineSolver, 
	// point B = shared point, 
	// normal = the cutting line's outward facing normal.
	CutLine cuttingCutLine(determinedCuttingPointToUse, sharedPoint, cuttingLineNormal);
	std::cout << "Cutting cut line stats: " << std::endl;
	std::cout << "Cutting, point A: " << determinedCuttingPointToUse.x << ", " << determinedCuttingPointToUse.y << ", " << determinedCuttingPointToUse.z << std::endl;
	std::cout << "Cutting, point B: " << sharedPoint.x << ", " << sharedPoint.y << ", " << sharedPoint.z << std::endl;
	std::cout << "Cutting, normal: " << cuttingLineNormal.x << ", "
		<< cuttingLineNormal.y << ", "
		<< cuttingLineNormal.z << std::endl;








	// the CuttableLine spawned by the CuttingLine should be:
	// point A = shared point,
	// point B = selected end point from cuttingLineSolver
	// normal = the cuttable line's centroid facing normal.
	CutLine cuttableCutLine(sharedPoint, determinedPoint, cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal);
	std::cout << "Cuttable cut line stats: " << std::endl;
	std::cout << "Cuttable, point A: " << sharedPoint.x << ", " << sharedPoint.y << ", " << sharedPoint.z << std::endl;
	std::cout << "Cuttable, point B: " << determinedPoint.x << ", " << determinedPoint.y << ", " << determinedPoint.z << std::endl;
	std::cout << "Cuttable, normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;

	// insert lines into pool, in appropriate order (cuttingCutLine, then cuttableCutLine)
	returnPool.insertLineIntoPool(cuttingCutLine);
	returnPool.insertLineIntoPool(cuttableCutLine);

	return returnPool;
}
CutLinePool CuttableTriangle::buildLinesFromSliceAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef)
{
	CutLinePool returnPool;

	return returnPool;
}

void CuttableTriangle::compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef)
{

}

void CuttableTriangle::printCuttableLineIntersections()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << "Data for line with index " << x << ": " << std::endl;
		cuttableTriangleLines[x].printCuttingIntersections();
	}
}

glm::vec3 CuttableTriangle::convert2DpointTo3D(TwoDPoint in_2dPoint)
{
	glm::vec3 returnVec;
	returnVec.x = in_2dPoint.x;
	returnVec.y = in_2dPoint.y;
	return returnVec;
}