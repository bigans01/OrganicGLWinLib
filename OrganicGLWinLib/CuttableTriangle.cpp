#include "stdafx.h"
#include "CuttableTriangle.h"

CuttableTriangle::CuttableTriangle(STriangle in_cuttableTriangle)
{


	// find the centroid
	glm::vec3 pointA = in_cuttableTriangle.triangleLines[0].pointA;
	glm::vec3 pointB = in_cuttableTriangle.triangleLines[1].pointA;
	glm::vec3 pointC = in_cuttableTriangle.triangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	//std::cout << "Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
	//std::cout << "Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
	//std::cout << "Point C: " << pointC.x << ", " << pointC.y << ", " << pointC.z << std::endl;

	//std::cout << "Centroid value is: " << centroid.x << ", " << centroid.y << ", " << centroid.z << std::endl;

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{
		QuatRotationPoints points;	// point 0 = pointA of line
									// point 1 = pointB of line
									// point 2 = centroid
		glm::vec3 pointACopy = in_cuttableTriangle.triangleLines[x].pointA;
		glm::vec3 pointBCopy = in_cuttableTriangle.triangleLines[x].pointB;
		glm::vec3 centroidCopy = centroid;
		points.insertPointRefs(&pointACopy, &pointBCopy, &centroidCopy);

		QMVec3FindCentroidFacingNormal normalFinder;
		glm::vec3 centroidFacingNormal = normalFinder.solve(&points, PolyDebugLevel::NONE);
		CuttableTriangleLine newLine(in_cuttableTriangle.triangleLines[x].pointA, in_cuttableTriangle.triangleLines[x].pointB, centroidFacingNormal);
		cuttableTriangleLines[x] = newLine;

		//std::cout << "############## Centroid facing normal, for index " << x << "for this cuttableTriangle, is: " <<
			//centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;
	}
}

void CuttableTriangle::compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef, int in_cuttingTriangleID, PolyDebugLevel in_polyDebugLevel)
{
	PolyLogger tempLogger;
	tempLogger.setDebugLevel(in_polyDebugLevel);
	tempLogger.log("(CuttableTriangle) beginning run of comparison against the CuttingTriangle, having index ", in_cuttingTriangleID, ".", "\n");

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

				//std::cout << ":::: cuttableSegment points: A> " << cuttableSegment.a.x << ", " << cuttableSegment.a.y << " | B> " << cuttableSegment.b.x << ", " << cuttableSegment.b.y << std::endl;
				//std::cout << ":::: cuttingSegment points: A> " << cuttingSegment.a.x << ", " << cuttingSegment.a.y << " | B> " << cuttingSegment.b.x << ", " << cuttingSegment.b.y << std::endl;
				tempLogger.log("(CuttableTriangle) :::: cuttableSegment points: A> ", cuttableSegment.a.x, ", ", cuttableSegment.a.y, " | B> ", cuttableSegment.b.x, ", ", cuttableSegment.b.y, "\n");
				tempLogger.log("(CuttableTriangle) :::: cuttingSegment points: A> ", cuttingSegment.a.x, ", ", cuttingSegment.a.y, " | B> ", cuttingSegment.b.x, ", ", cuttingSegment.b.y, "\n");

				TwoDLineSegmentIntersectAnalyzerV2 analyzerV2(cuttableSegment, cuttingSegment, PolyDebugLevel::NONE);
				if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
				{
					glm::vec3 convertedPoint = convert2DpointTo3D(analyzerV2.analyzedResult.intersectedPoint);
					// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
					cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, convertedPoint);

					// find the point which wasn't intersected, store it.
					QuatRotationPoints solveForCuttableLinePoints;
					glm::vec3 solveForCuttablecuttingLinePointA = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
					glm::vec3 solveForCuttablecuttingLinePointB = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 solveForCuttablecuttableLinePointA = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					glm::vec3 solveForCuttablecuttableLinePointB = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					glm::vec3 solveForCuttableLineInwardNormal = cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal;
					solveForCuttableLinePoints.insertPointRefs(&solveForCuttablecuttingLinePointA,
															&solveForCuttablecuttingLinePointB,
															&solveForCuttablecuttableLinePointA,
															&solveForCuttablecuttableLinePointB,
															&solveForCuttableLineInwardNormal);
					QMVec3FindCyclingDirectionPoint cuttableLineSolver;
					glm::vec3 determinedNonIntersectingPoint = cuttableLineSolver.solve(&solveForCuttableLinePoints, PolyDebugLevel::NONE);

					// store the non intersecting point for this cutting line.
					cuttableTriangleLines[currentCuttableTriangleLineID].insertNonIntersectingCuttingLinePoint(currentCuttingTriangleLineID, determinedNonIntersectingPoint);


					// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
					in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, convertedPoint);

					//std::cout << "!!! Non-colinear intersection detected; cuttable ID is: " << currentCuttableTriangleLineID
						//<< " | cutting ID is: " << currentCuttingTriangleLineID
						//<< " | point is: " << analyzerV2.analyzedResult.intersectedPoint.x << ", " << analyzerV2.analyzedResult.intersectedPoint.y << std::endl;
					//int outputVal = 3;
					//std::cin >> outputVal;

					tempLogger.log("(CuttableTriangle) !!! Non-colinear intersection detected; cuttable ID is: ", currentCuttableTriangleLineID,
						" | cutting ID is: ", currentCuttingTriangleLineID,
						" | point is: ", analyzerV2.analyzedResult.intersectedPoint.x, ", ", analyzerV2.analyzedResult.intersectedPoint.y, "\n");
					tempLogger.waitForDebugInput();

 
				}
				//else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_A)
				//{
					//glm::vec3 pointToCheck = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					//glm::vec3 splitLinePointA = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					//glm::vec3 splitLinePointB = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					//glm::vec3 splitLineNormal = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].outwardFacingNormal;
				//}
				//else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_B)
				//{

				//}
				else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_B_SPLITS_A_VIA_POINT_A)
				{
					//std::cout << "######::: NOTICE, running T-junction validity test for T_JUNCTION_B_SPLITS_A_VIA_POINT_A. " << std::endl;
					//glm::vec3 interestedPointToCheck = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					//std::cout << "Point to check will be: " << interestedPointToCheck.x << ", " << interestedPointToCheck.y << ", " << interestedPointToCheck.z << std::endl;
					//int tJunctionWait = 3;
					//std::cin >> tJunctionWait;

					tempLogger.log("(CuttableTriangle) ######::: NOTICE, running T-junction validity test for T_JUNCTION_B_SPLITS_A_VIA_POINT_A. ", "\n");
					tempLogger.log("(CuttableTriangle) Checking this point for validity: ",
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.x, ", ",
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.y, "\n");
					tempLogger.waitForDebugInput();


					// point A of line B splits line A; so we must check point B of line B.
					glm::vec3 splitLinePointA = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
					glm::vec3 splitLinePointB = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 pointToCheck = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					glm::vec3 splitLineNormal = cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal;
					QuatRotationPoints tJunctionPoints;
					tJunctionPoints.insertPointRefs(&splitLinePointA,
													&splitLinePointB,
													&pointToCheck,
													&splitLineNormal);

					// run a bool QM machine to test whether or not pointToCheck is in the same direction as the splitLineNormal.
					// If true, we can insert a record.
					QMBoolIsTJunctionCuttable cuttableTester;
					bool result = cuttableTester.solve(&tJunctionPoints, PolyDebugLevel::NONE);
					if (result == true)
					{
						std::cout << ":::::::::: T-junction is VALID; inserting..." << std::endl;
						// since point A of the cutting line was the point that split line A, use it as the intersecting point.
						glm::vec3 pointToUse = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
						// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
						cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, pointToUse);

						// the non-intersecting point will be equal to point B of the cuttling line.
						glm::vec3 cuttingLineNonIntersectingPoint = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
						cuttableTriangleLines[currentCuttableTriangleLineID].insertNonIntersectingCuttingLinePoint(currentCuttingTriangleLineID, cuttingLineNonIntersectingPoint);

						// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, pointToUse);
					}
					
				

				}
				else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_B_SPLITS_A_VIA_POINT_B)
				{
					//std::cout << "######::: NOTICE, running T-junction validity test for T_JUNCTION_B_SPLITS_A_VIA_POINT_B. " << std::endl;
					//glm::vec3 interestedPointToCheck = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					//std::cout << "Point to check will be: " << interestedPointToCheck.x << ", " << interestedPointToCheck.y << ", " << interestedPointToCheck.z << std::endl;
					//int tJunctionWait = 3;
					//std::cin >> tJunctionWait;

					tempLogger.log("(CuttableTriangle) ######::: NOTICE, running T-junction validity test for T_JUNCTION_B_SPLITS_A_VIA_POINT_B. ", "\n");
					tempLogger.log("(CuttableTriangle) Checking this point for validity: ",
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.x, ", ",
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.y, "\n");
					tempLogger.waitForDebugInput();

					// point B of line B splits line A; so we must check point A of line B.
					glm::vec3 splitLinePointA = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
					glm::vec3 splitLinePointB = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 pointToCheck = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					glm::vec3 splitLineNormal = cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal;
					QuatRotationPoints tJunctionPoints;
					tJunctionPoints.insertPointRefs(&splitLinePointA,
													&splitLinePointB,
													&pointToCheck,
													&splitLineNormal);

					// run a bool QM machine to test whether or not pointToCheck is in the same direction as the splitLineNormal.
					// If true, we can insert a record.
					QMBoolIsTJunctionCuttable cuttableTester;
					bool result = cuttableTester.solve(&tJunctionPoints, PolyDebugLevel::NONE);
					if (result == true)
					{
						std::cout << ":::::::::: T-junction is VALID; inserting..." << std::endl;
						// since point B of the cutting line was the point that split line A, use it as the intersecting point.
						glm::vec3 pointToUse = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
						// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
						cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, pointToUse);

						// the non-intersecting point will be equal to point A of the cuttling line.
						glm::vec3 cuttingLineNonIntersectingPoint = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
						cuttableTriangleLines[currentCuttableTriangleLineID].insertNonIntersectingCuttingLinePoint(currentCuttingTriangleLineID, cuttingLineNonIntersectingPoint);

						// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, pointToUse);
					}
				}


				// T_junction logic needs to be handled here....
			}
		}

		// second, check for any slicing conditions that might have been met -- this would happen when any cutting line in the cutting triangle has 2 intersections
		// registered to it. Build the slicing attempt (via an instance of TwoDCrawlingAttempt),
		// and update the cuttableIntersetionManagers, by removing records that refer to the cutting line which had 2 lines.
		buildAllSlicingAttempts(in_cuttingTriangleRef);

		// third, find the first of any other cuttingTriangleLines which have 1 intersection, if they exist; these would be typical attempts.
		buildTypicalAttempts(in_cuttingTriangleRef);

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
			//cuttableTriangleLines[intersectionRecordsBegin->first].cuttableIntersectionManager.eraseRecord(x);
			//cuttableTriangleLines[intersectionRecordsEnd->first].cuttableIntersectionManager.eraseRecord(x);

			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for beginIntersectionLineID: " << cuttableTriangleLines[beginIntersectionLineID->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for intersectionRecordsEnd: " << cuttableTriangleLines[intersectionRecordsEnd->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
		}
	}
}

void CuttableTriangle::buildTypicalAttempts(CuttingTriangle* in_cuttingTriangleRef)
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
		PoolAndDirectionPair currentPair;
		if (attemptsBegin->crawlingType == TwoDCrawlingType::SLICE)
		{
			std::cout << "SLICE crawl type found..." << std::endl;
			currentPair = buildLinesFromSliceAttempt(*attemptsBegin, in_cuttingTriangleRef);
		}
		else if (attemptsBegin->crawlingType == TwoDCrawlingType::TYPICAL)
		{
			std::cout << "TYPICAL crawl type found..." << std::endl;
			currentPair = buildLinesFromTypicalAttempt(*attemptsBegin, in_cuttingTriangleRef);
		}

		// run a CutLineWelder, take its produced pool and use it in the CutTriangleGroupBuilder.
		CutLineWelder welder(this, in_cuttingTriangleRef, *attemptsBegin, currentPair.pairPool, currentPair.pairCyclingDirection);
		CutTriangleGroupBuilder builder(PolyDebugLevel::NONE, welder.currentPool);
		builder.runCutTraceObserver();
		convertAndStoreCutTriangleVector(std::move(builder.produceAndReturnCutTriangleVector()));
		// extract all the CutTriangles from the CutTriangleGroupBuilder.

		
		std::cout << "------------ended trace observer run. " << std::endl;
		auto containerVectorBegin = builder.cutTriangleContainerVector.begin();
		auto containerVectorEnd = builder.cutTriangleContainerVector.end();
		for (; containerVectorBegin != containerVectorEnd; containerVectorBegin++)
		{
			std::cout << ">>>>> Printing contents for container..." << std::endl;
			auto containerTrianglesBegin = containerVectorBegin->cutTrianglesMap.begin();
			auto containerTrianglesEnd = containerVectorBegin->cutTrianglesMap.end();
			for (; containerTrianglesBegin != containerTrianglesEnd; containerTrianglesBegin++)
			{
				containerTrianglesBegin->second.printPoints();
			}
		}


		
	}
}

void CuttableTriangle::convertAndStoreCutTriangleVector(std::vector<CutTriangle> in_vector)
{
	auto vectorBegin = in_vector.begin();
	auto vectorEnd = in_vector.end();
	for (; vectorBegin != vectorEnd; vectorBegin++)
	{
		STriangle convertedTriangle = convertCutTriangleToSTriangle(*vectorBegin);
		int currentOutputMapSize = outputTriangles.size();
		outputTriangles[currentOutputMapSize] = convertedTriangle;
	}
}

STriangle CuttableTriangle::convertCutTriangleToSTriangle(CutTriangle in_cutTriangle)
{
	STriangle returnSTriangle(in_cutTriangle.lines[0].pointA, in_cutTriangle.lines[1].pointA, in_cutTriangle.lines[2].pointA);
	return returnSTriangle;
}

void CuttableTriangle::printCuttableTrianglePoints()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << "point " << x << ": ";
		std::cout << cuttableTriangleLines[x].pointA.x << ", " << cuttableTriangleLines[x].pointA.y << ", " << cuttableTriangleLines[x].pointA.z << std::endl;
	}
}

CuttableTriangle::PoolAndDirectionPair CuttableTriangle::buildLinesFromTypicalAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef)
{
	//CutLinePool returnPool;
	PoolAndDirectionPair returnPair;

	// common point between both lines:
	glm::vec3 sharedPoint = in_attempt.beginIntersectingPoint;

	// First, find the cyling direction. 
	// line A point values, from the CuttingLine
	glm::vec3 cuttingLinePointA = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointA;
	glm::vec3 cuttingLinePointB = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointB;

	// outward facing normal
	glm::vec3 cuttingLineNormal = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].outwardFacingNormal;
	int idOfLineFoundInCuttingLine = in_attempt.beginIntersectionLineID;

	// line B point values, from the CuttableLine
	glm::vec3 cuttableLinePointA = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointA;
	glm::vec3 cuttableLinePointB = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointB;

	std::cout << "::> cutting line point A: " << cuttingLinePointA.x << ", " << cuttingLinePointA.y << ", " << cuttingLinePointA.z << std::endl;
	std::cout << "::> cutting line point B: " << cuttingLinePointB.x << ", " << cuttingLinePointB.y << ", " << cuttingLinePointB.z << std::endl;
	std::cout << "::> cuttable line point A: " << cuttableLinePointA.x << ", " << cuttableLinePointA.y << ", " << cuttableLinePointA.z << std::endl;
	std::cout << "::> cuttable line point B: " << cuttableLinePointB.x << ", " << cuttableLinePointB.y << ", " << cuttableLinePointB.z << std::endl;
	// two quat machines for finding the points: one for the cutting line, one for the cuttable line

	QuatRotationPoints solveForCuttableLinePoints;
	glm::vec3 solveForCuttablecuttingLinePointA = cuttableLinePointA;
	glm::vec3 solveForCuttablecuttingLinePointB = cuttableLinePointB;
	glm::vec3 solveForCuttablecuttableLinePointA = cuttingLinePointA;
	glm::vec3 solveForCuttablecuttableLinePointB = cuttingLinePointB;
	glm::vec3 solveForCuttableLineInwardNormal = cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal;
	solveForCuttableLinePoints.insertPointRefs(&solveForCuttablecuttingLinePointA,
												&solveForCuttablecuttingLinePointB,
												&solveForCuttablecuttableLinePointA,
												&solveForCuttablecuttableLinePointB,
												&solveForCuttableLineInwardNormal);
	QMVec3FindCyclingDirectionPoint cuttableLineSolver;
	glm::vec3 determinedCuttingPointToUse = cuttableLineSolver.solve(&solveForCuttableLinePoints, PolyDebugLevel::NONE);
	std::cout << "::> Cuttable line normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;
	std::cout << "::> Cutting point to use, determined by cuttable line: " << determinedCuttingPointToUse.x << ", " << determinedCuttingPointToUse.y << ", " << determinedCuttingPointToUse.z << std::endl;




	QuatRotationPoints solveForCuttingLinePoints;
	glm::vec3 solveForCuttingcuttingLinePointA = cuttingLinePointA;
	glm::vec3 solveForCuttingcuttingLinePointB = cuttingLinePointB;
	glm::vec3 solveForCuttingcuttableLinePointA = cuttableLinePointA;
	glm::vec3 solveForCuttingcuttableLinePointB = cuttableLinePointB;
	glm::vec3 solveForCuttingOutwardNormal = cuttingLineNormal;
	solveForCuttingLinePoints.insertPointRefs(&solveForCuttingcuttingLinePointA,
												&solveForCuttingcuttingLinePointB,
												&solveForCuttingcuttableLinePointA,
												&solveForCuttingcuttableLinePointB,
												&solveForCuttingOutwardNormal);
	QMVec3FindCyclingDirectionPoint cuttingLineSolver;
	glm::vec3 cuttablePointToUse = cuttingLineSolver.solve(&solveForCuttingLinePoints, PolyDebugLevel::NONE);
	std::cout << "::> Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;
	std::cout << "::> Cuttable point to use, determined by cutting line: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;

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
	CutLine cuttableCutLine(sharedPoint, cuttablePointToUse, cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal);
	std::cout << "Cuttable cut line stats: " << std::endl;
	std::cout << "Cuttable, point A: " << sharedPoint.x << ", " << sharedPoint.y << ", " << sharedPoint.z << std::endl;
	std::cout << "Cuttable, point B: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;
	std::cout << "Cuttable, normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;

	// insert lines into pool, in appropriate order (cuttingCutLine, then cuttableCutLine)
	returnPair.pairPool.insertLineIntoPool(cuttingCutLine);
	returnPair.pairPool.insertLineIntoPool(cuttableCutLine);

	// determine the cycling direction value to use, which will be based on the value of cuttablePointToUse.
	CyclingDirection cutLineDirection = CyclingDirection::NOVAL;
	if (cuttablePointToUse == cuttableLinePointA)
	{
		std::cout << "!! CyclingDirection will be BACKWARD." << std::endl;
		cutLineDirection = CyclingDirection::REVERSE;
	}
	else if (cuttablePointToUse == cuttableLinePointB)
	{
		std::cout << "!! CyclingDirection will be FORWARD." << std::endl;
		cutLineDirection = CyclingDirection::FORWARD;
	}
	returnPair.pairCyclingDirection = cutLineDirection;

	return returnPair;
}
CuttableTriangle::PoolAndDirectionPair CuttableTriangle::buildLinesFromSliceAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef)
{
	PoolAndDirectionPair returnPair;

	// common point between both lines:
	glm::vec3 sharedPoint = in_attempt.beginIntersectingPoint;

	// First, find the cyling direction. 
	// line A point values, from the CuttingLine
	glm::vec3 cuttingLinePointA = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointA;
	glm::vec3 cuttingLinePointB = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].pointB;

	// outward facing normal
	glm::vec3 cuttingLineNormal = in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].outwardFacingNormal;
	int idOfLineFoundInCuttingLine = in_attempt.beginIntersectionLineID;

	// line B point values, from the CuttableLine
	glm::vec3 cuttableLinePointA = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointA;
	glm::vec3 cuttableLinePointB = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointB;

	std::cout << "::> cutting line point A: " << cuttingLinePointA.x << ", " << cuttingLinePointA.y << ", " << cuttingLinePointA.z << std::endl;
	std::cout << "::> cutting line point B: " << cuttingLinePointB.x << ", " << cuttingLinePointB.y << ", " << cuttingLinePointB.z << std::endl;
	std::cout << "::> cuttable line point A: " << cuttableLinePointA.x << ", " << cuttableLinePointA.y << ", " << cuttableLinePointA.z << std::endl;
	std::cout << "::> cuttable line point B: " << cuttableLinePointB.x << ", " << cuttableLinePointB.y << ", " << cuttableLinePointB.z << std::endl;
	// two quat machines for finding the points: one for the cutting line, one for the cuttable line

	QuatRotationPoints solveForCuttingLinePoints;
	glm::vec3 solveForCuttingcuttingLinePointA = cuttingLinePointA;
	glm::vec3 solveForCuttingcuttingLinePointB = cuttingLinePointB;
	glm::vec3 solveForCuttingcuttableLinePointA = cuttableLinePointA;
	glm::vec3 solveForCuttingcuttableLinePointB = cuttableLinePointB;
	glm::vec3 solveForCuttingOutwardNormal = cuttingLineNormal;
	solveForCuttingLinePoints.insertPointRefs(&solveForCuttingcuttingLinePointA,
												&solveForCuttingcuttingLinePointB,
												&solveForCuttingcuttableLinePointA,
												&solveForCuttingcuttableLinePointB,
												&solveForCuttingOutwardNormal);
	QMVec3FindCyclingDirectionPoint cuttingLineSolver;
	glm::vec3 cuttablePointToUse = cuttingLineSolver.solve(&solveForCuttingLinePoints, PolyDebugLevel::NONE);
	std::cout << "::> Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;
	std::cout << "::> Cuttable point to use, determined by cutting line: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;

	// the CuttableLine spawned by the CuttingLine should be:
	// point A = shared point,
	// point B = selected end point from cuttingLineSolver
	// normal = the cuttable line's centroid facing normal.
	CutLine cuttableCutLine(sharedPoint, cuttablePointToUse, cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal);
	std::cout << "Cuttable cut line stats: " << std::endl;
	std::cout << "Cuttable, point A: " << sharedPoint.x << ", " << sharedPoint.y << ", " << sharedPoint.z << std::endl;
	std::cout << "Cuttable, point B: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;
	std::cout << "Cuttable, normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;

	// for a SLICE attempt, the first line is always going to be the sliced line.
	CutLine slicedCutLine(in_attempt.endIntersectingPoint, in_attempt.beginIntersectingPoint, in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].outwardFacingNormal);

	// the second line will be a line where point A = the begin intersecting point, point B = the cuttablePointToUse, and the normal is the centroid facing normal from the cuttable line.
	returnPair.pairPool.insertLineIntoPool(slicedCutLine);
	returnPair.pairPool.insertLineIntoPool(cuttableCutLine);

	std::cout << "||||||||||||||||| printing CutLinePool, after SLICE attempt build: " << std::endl;
	returnPair.pairPool.printLines();

	// determine the cycling direction value to use, which will be based on the value of cuttablePointToUse.
	CyclingDirection cutLineDirection = CyclingDirection::NOVAL;
	if (cuttablePointToUse == cuttableLinePointA)
	{
		std::cout << "!! Slice attempt -- CyclingDirection will be BACKWARD." << std::endl;
		cutLineDirection = CyclingDirection::REVERSE;
	}
	else if (cuttablePointToUse == cuttableLinePointB)
	{
		std::cout << "!! Slice attempt -- CyclingDirection will be FORWARD." << std::endl;
		cutLineDirection = CyclingDirection::FORWARD;
	}
	returnPair.pairCyclingDirection = cutLineDirection;

	return returnPair;
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