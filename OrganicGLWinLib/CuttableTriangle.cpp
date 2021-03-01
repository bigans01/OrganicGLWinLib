#include "stdafx.h"
#include "CuttableTriangle.h"

CuttableTriangle::CuttableTriangle(STriangle in_cuttableTriangle)
{
	// find the centroid
	glm::vec3 pointA = in_cuttableTriangle.triangleLines[0].pointA;
	glm::vec3 pointB = in_cuttableTriangle.triangleLines[1].pointA;
	glm::vec3 pointC = in_cuttableTriangle.triangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{			
		glm::vec3 centroidFacingNormal = QuatUtils::findOrientatedLineNormal(in_cuttableTriangle.triangleLines[x].pointA, in_cuttableTriangle.triangleLines[x].pointB, centroid);
		CuttableTriangleLine newLine(in_cuttableTriangle.triangleLines[x].pointA, in_cuttableTriangle.triangleLines[x].pointB, centroidFacingNormal);
		cuttableTriangleLines[x] = newLine;
	}
}

void CuttableTriangle::compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef, int in_cuttingTriangleID, DebugOptionSet in_cuttingTriangledebugOptionSet)
{
	PolyLogger tempLogger;
	tempLogger.setDebugLevel(PolyDebugLevel::DEBUG);
	tempLogger.log("(CuttableTriangle) beginning run of comparison against the CuttingTriangle, having index ", in_cuttingTriangleID, ".", "\n");



	// perform consumption tests; if they fail, continue with normal operations in final branch.
	// do all points of the CuttableTriangle lie within the CuttingTriangle?
	if (testIfCuttingTriangleConsumesThisTriangle(in_cuttingTriangleRef) == true)
	{
		// we should be done if this is the case.
		std::cout << "******** CONSUMPTION DETECTED. " << std::endl;
	}

	// otherwise, attempt to generate attempts as normal.
	else
	{
		std::cout << "******** BEGIN LINE ANALYSIS. " << std::endl;


		// first, run each CuttableTriangleLine against each CuttingTriangleLine.
		for (int currentCuttableTriangleLineID = 0; currentCuttableTriangleLineID < 3; currentCuttableTriangleLineID++)
		{
			std::cout << ":::::::::::::::::::!!! Printing out cuttable/cutting triangle points: " << std::endl;
			std::cout << "Cuttables: " << std::endl;
			printCuttableTrianglePoints();
			std::cout << "Cutting: " << std::endl;
			in_cuttingTriangleRef->printPoints();

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

				std::cout << "#####:::: cuttableSegment points: A> " << cuttableSegment.a.x << ", " << cuttableSegment.a.y << " | B> " << cuttableSegment.b.x << ", " << cuttableSegment.b.y << std::endl;
				std::cout << "#####:::: cuttingSegment points: A> " << cuttingSegment.a.x << ", " << cuttingSegment.a.y << " | B> " << cuttingSegment.b.x << ", " << cuttingSegment.b.y << std::endl;
				//tempLogger.log("(CuttableTriangle) :::: cuttableSegment points: A> ", cuttableSegment.a.x, ", ", cuttableSegment.a.y, " | B> ", cuttableSegment.b.x, ", ", cuttableSegment.b.y, "\n");
				//tempLogger.log("(CuttableTriangle) :::: cuttingSegment points: A> ", cuttingSegment.a.x, ", ", cuttingSegment.a.y, " | B> ", cuttingSegment.b.x, ", ", cuttingSegment.b.y, "\n");

				TwoDLineSegmentIntersectAnalyzerV2 analyzerV2(cuttableSegment, cuttingSegment, PolyDebugLevel::NONE);
				if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
				{
					glm::vec3 convertedPoint = convert2DpointTo3D(analyzerV2.analyzedResult.intersectedPoint);
					// get the third, unused point; use it to check if there are actually two points which are coplanar to the cutting line.
					CuttableTriangle::CuttablePointPair pointPair(convert2DpointTo3D(cuttableSegment.a), convert2DpointTo3D(cuttableSegment.b));
					glm::vec3 thirdUnusedPoint = fetchThirdPoint(pointPair);
					CuttableTriangle::PotentialLineColinearityResult colinearityResult = acquireColinearityResult(thirdUnusedPoint,
																										convertedPoint,
																										in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA,
																										in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB);
																												

					std::cout << "###!!! Third unused point is: " << thirdUnusedPoint.x << ", " << thirdUnusedPoint.y << ", " << thirdUnusedPoint.z << std::endl;


					// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
					cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, convertedPoint);

					// find the point which wasn't intersected, store it; this will be used to help determine the cycling direction, as well as determining
					// point B for the line to insert.
					glm::vec3 determinedNonIntersectingPoint = QuatUtils::findPointForDeterminingCyclingDirection(
																			cuttableTriangleLines[currentCuttableTriangleLineID].pointA,
																			cuttableTriangleLines[currentCuttableTriangleLineID].pointB,
																			in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA,
																			in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB,
																			cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal
																		).resultPoint;



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

				// ||||||||||||||||: T-junction checks begin here 
				// For reference, the "tJunctionBasePoint" is the point in a T-junction that represents the bottom of the "T"; it is the nonintersecting point 
				// of the line that causes the split (the other point is the one that actually causes the junction)
				else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_A)
				{
					//glm::vec3 pointToCheck = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 splitLinePointA = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					glm::vec3 splitLinePointB = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					glm::vec3 tJunctionBasePoint = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 splitLineNormal = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].outwardFacingNormal;

					// the splitLineNormal to use for T_JUNCTION_A_SPLITS_B_VIA_POINT_A must be the inverse of the  outwardFacingNormal, 
					// since the tJunctionBasePoint would have to fall WITHIN the CuttingTriangle's area (PBZ) to actually be used.
					bool result = QuatUtils::checkTJunctionUsability(splitLinePointA, splitLinePointB, tJunctionBasePoint, splitLineNormal*=-1);
					if (result == true)
					{
						std::cout << ":::::::::: T-junction is VALID; inserting..." << std::endl;
						// since point A of the cutting line was the point that split line A, use it as the intersecting point.
						glm::vec3 pointToUse = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
						// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
						cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, pointToUse);

						// the non-intersecting point will be equal to point B of the cuttling line.
						glm::vec3 cuttingLineNonIntersectingPoint = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
						cuttableTriangleLines[currentCuttableTriangleLineID].insertNonIntersectingCuttingLinePoint(currentCuttingTriangleLineID, cuttingLineNonIntersectingPoint);

						// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
						std::cout << "::::::: point to use, for T-junction insert: " << pointToUse.x << ", " << pointToUse.y << ", " << pointToUse.z << std::endl;
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, pointToUse);

						std::cout << ":::::::::: Finished T-junction insert. " << std::endl;
						int finishVal = 3;
						std::cin >> finishVal;
					}
				}
				else if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::T_JUNCTION_A_SPLITS_B_VIA_POINT_B)
				{
					glm::vec3 splitLinePointA = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					glm::vec3 splitLinePointB = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					glm::vec3 tJunctionBasePoint = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
					glm::vec3 splitLineNormal = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].outwardFacingNormal;

					// the splitLineNormal to use for T_JUNCTION_A_SPLITS_B_VIA_POINT_B must be the inverse of the  outwardFacingNormal, 
					// since the tJunctionBasePoint would have to fall WITHIN the CuttingTriangle's area (PBZ) to actually be used.
					bool result = QuatUtils::checkTJunctionUsability(splitLinePointA, splitLinePointB, tJunctionBasePoint, splitLineNormal*=-1);
					if (result == true)
					{
						std::cout << ":::::::::: T-junction is VALID; inserting..." << std::endl;
						// since point B of the cutting line was the point that split line A, use it as the intersecting point.
						glm::vec3 pointToUse = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
						// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
						cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, pointToUse);

						// the non-intersecting point will be equal to point B of the cuttling line.
						glm::vec3 cuttingLineNonIntersectingPoint = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
						cuttableTriangleLines[currentCuttableTriangleLineID].insertNonIntersectingCuttingLinePoint(currentCuttingTriangleLineID, cuttingLineNonIntersectingPoint);

						// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
						std::cout << "::::::: point to use, for T-junction insert: " << pointToUse.x << ", " << pointToUse.y << ", " << pointToUse.z << std::endl;
						in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, pointToUse);

						std::cout << ":::::::::: Finished T-junction insert. " << std::endl;
						int finishVal = 3;
						std::cin >> finishVal;
					}
				}
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

					// run a bool QM machine to test whether or not pointToCheck is in the same direction as the splitLineNormal.
					// If true, we can insert a record.
					// point A of line B splits line A; so we must check point B of line B.
					glm::vec3 splitLinePointA = cuttableTriangleLines[currentCuttableTriangleLineID].pointA;
					glm::vec3 splitLinePointB = cuttableTriangleLines[currentCuttableTriangleLineID].pointB;
					glm::vec3 tJunctionBasePoint = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB;
					glm::vec3 splitLineNormal = cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal;
					bool result = QuatUtils::checkTJunctionUsability(splitLinePointA, splitLinePointB, tJunctionBasePoint, splitLineNormal);
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

						std::cout << ":::::::::: Finished T-junction insert. " << std::endl;
						int finishVal = 3;
						std::cin >> finishVal;
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
					glm::vec3 tJunctionBasePoint = in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA;
					glm::vec3 splitLineNormal = cuttableTriangleLines[currentCuttableTriangleLineID].cuttableTriangleCentroidFacingNormal;
					bool result = QuatUtils::checkTJunctionUsability(splitLinePointA, splitLinePointB, tJunctionBasePoint, splitLineNormal);
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

						std::cout << ":::::::::: Finished T-junction insert. " << std::endl;
						int finishVal = 3;
						std::cin >> finishVal;
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

		std::cout << "!!!! Size of attempts: " << crawlingAttemptsVector.size() << std::endl;
		int attemptsWait = 3;
		std::cin >> attemptsWait;

		// fourth: test whether or not this triangle consumes the CuttingTriangle; this is only true when:
		// A) all points of the CuttingTriangle are within the CuttableTriangle
		// B) there are no attempts built, meaning there are 0 intersections.
		//
		// If true, do special logic. Otherwise, if false, produce cut line pools as normal.
		if    // the CuttableTriangle actually consumes the CuttingTriangle ("engulfing" style) 
		(
			(testIfThisTriangleConsumesCuttingTriangle(in_cuttingTriangleRef) == true)
			&&
			(crawlingAttemptsVector.size() == 0)
		)
		{
			// special logic.
		}
		else if (crawlingAttemptsVector.size() != 0)	// the CuttableTriangle will be modified
		{
			// fourth, build the CutLinePools from each attempt.
			produceCutLinePoolsFromAttempts(in_cuttingTriangleRef);
		}
		else if (crawlingAttemptsVector.size() == 0)	// there weren't any intersections; the CuttableTriangle is unmodified, so just use the 
														// CuttableTriangle and it's original points to form an STriangle, and put it into
														// the output STriangle vector.
		{
			STriangle returnSTriangle(cuttableTriangleLines[0].pointA, cuttableTriangleLines[1].pointA, cuttableTriangleLines[2].pointA);
			outputTriangles[0] = returnSTriangle;
		}
	}
}

glm::vec3 CuttableTriangle::fetchThirdPoint(CuttablePointPair in_cuttablePointPair)
{
	glm::vec3 returnPoint;
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 pointToCheckFor = cuttableTriangleLines[x].pointA;
		bool wasMatchFound = false;
		for (int y = 0; y < 2; y++)
		{
			if (pointToCheckFor == in_cuttablePointPair.pointArray[y])
			{
				wasMatchFound = true;
				break;
			}
		}

		if (wasMatchFound == false)
		{
			returnPoint = pointToCheckFor;
		}
	}
	return returnPoint;
}

CuttableTriangle::PotentialLineColinearityResult CuttableTriangle::acquireColinearityResult(glm::vec3 in_thirdPoint, glm::vec3 in_intersectingPoint, glm::vec3 in_cuttingLinePointA, glm::vec3 in_cuttingLinePointB)
{
	PotentialLineColinearityResult colinearityResult;
	bool areLinesColinear = QuatUtils::checkIfLinesAreColinear(in_thirdPoint, in_intersectingPoint, in_cuttingLinePointA, in_cuttingLinePointB);
	if (areLinesColinear == true)
	{
		std::cout << "----> CoLinearity result: lines are colinear; waiting for input to continue. " << std::endl;
		int waitVal = 3; 
		std::cin >> waitVal;
	}
	return colinearityResult;
}

bool CuttableTriangle::testIfCuttingTriangleConsumesThisTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	bool isCuttingTriangleConsumed = false;

	std::cout << ":::::::::::::: START PBZ Test: " << std::endl;
	std::cout << "Cuttable points are: " << std::endl;
	printCuttableTrianglePoints();
	std::cout << "Cutting triangle points are: " << std::endl;
	in_cuttingTriangleRef->printPoints();
	int waitForPrint = 3;
	std::cin >> waitForPrint;
	
	// compare all lines of the cuttable,
	for (int x = 0; x < 3; x++)
	{
		
		// to all lines of the cutting
		for (int y = 0; y < 3; y++)
		{
			glm::vec3 cuttablePointA = cuttableTriangleLines[x].pointA;
			glm::vec3 cuttablePointB = cuttableTriangleLines[x].pointB;
			glm::vec3 cuttingPointA = in_cuttingTriangleRef->cuttingLines[y].pointA;
			glm::vec3 cuttingPointB = in_cuttingTriangleRef->cuttingLines[y].pointB;

			std::cout << ":::::::: Comparison points: " << std::endl;
			std::cout << "Cuttable point A: " << cuttablePointA.x << ", " << cuttablePointA.y << ", " << cuttablePointA.z << std::endl;
			std::cout << "Cuttable point B: " << cuttablePointB.x << ", " << cuttablePointB.y << ", " << cuttablePointB.z << std::endl;
			std::cout << "Cutting point A: " << cuttingPointA.x << ", " << cuttingPointA.y << ", " << cuttingPointA.z << std::endl;
			std::cout << "Cutting point B: " << cuttingPointB.x << ", " << cuttingPointB.y << ", " << cuttingPointB.z << std::endl;

			//
			bool containmentDetected = QuatUtils::isLineAContainedWithinB(cuttablePointA, cuttablePointB, cuttingPointA, cuttingPointB);
			if (containmentDetected == true)
			{
				std::cout << "!!!!!!! NOTICE-> containment detected. Halting and waiting for input. " << std::endl;
				

				CuttableTriangle::CuttablePointPair pointPair(cuttablePointA, cuttablePointB);
				glm::vec3 thirdUnusedPoint = fetchThirdPoint(pointPair);
				glm::vec3 cuttingTrianglePoint0 = in_cuttingTriangleRef->cuttingLines[0].pointA;
				glm::vec3 cuttingTrianglePoint1 = in_cuttingTriangleRef->cuttingLines[1].pointA;
				glm::vec3 cuttingTrianglePoint2 = in_cuttingTriangleRef->cuttingLines[2].pointA;
				bool doesThirdPointLieWithinPBZ = QuatUtils::checkIfPointLiesWithinTrianglePBZ(
																									thirdUnusedPoint,
																									cuttingTrianglePoint0,
																									cuttingTrianglePoint1,
																									cuttingTrianglePoint2
																								);	
				if (doesThirdPointLieWithinPBZ == true)
				{
					std::cout << "!!!! NOTICE-> third point found as being within PBZ, this CuttableTriangle is completely cut! " << std::endl;
					isCuttingTriangleConsumed = true;
				}

				//int haltWait = 3;
				//std::cin >> haltWait;

				//isCuttingTriangleConsumed = true;
			}
			// if containment is detected, we must check the 3rd point.
		}
	}
	
	/*
	bool boolArray[3] = { false };
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 thirdUnusedPoint = cuttableTriangleLines[x].pointA;

		std::cout << "Point to check for index " << x << " will be: " << thirdUnusedPoint.x << ", " << thirdUnusedPoint.y << ", " << thirdUnusedPoint.z << std::endl;

		glm::vec3 cuttingTrianglePoint0 = in_cuttingTriangleRef->cuttingLines[0].pointA;
		glm::vec3 cuttingTrianglePoint1 = in_cuttingTriangleRef->cuttingLines[1].pointA;
		glm::vec3 cuttingTrianglePoint2 = in_cuttingTriangleRef->cuttingLines[2].pointA;
		boolArray[x] = QuatUtils::checkIfPointLiesWithinTrianglePBZ(
			thirdUnusedPoint,
			cuttingTrianglePoint0,
			cuttingTrianglePoint1,
			cuttingTrianglePoint2
		);

	}
	
	int boolCount = 0;
	for (int y = 0; y < 3; y++)
	{
		if (boolArray[y] == true)
		{
			std::cout << "Bool value at " << y << " was true!" << std::endl;
			boolCount++;
		}
	}
	if (boolCount == 3)
	{
		std::cout << "Notice::: Triangle is entirely contained within PBZ. " << std::endl;
		isCuttingTriangleConsumed = true;
	}
	*/

	std::cout << ":::::::::::::: END PBZ Test: " << std::endl;
	int endTest = 3;
	std::cin >> endTest;

	return isCuttingTriangleConsumed;
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
			// a slice run can only be run, as long as the two points involved aren't equal. 
			if (in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.arePointsEqual() == false)
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
}

void CuttableTriangle::buildTypicalAttempts(CuttingTriangle* in_cuttingTriangleRef)
{
	for (int a = 0; a < 3; a++)
	{
		std::cout << "Size of cutting lines, index " << a << " records: " << in_cuttingTriangleRef->cuttingLines[a].cuttingIntersectionManager.numberOfRecords() << std::endl;
	}
	int outputVal = 3;
	std::cin >> outputVal;

	for (int x = 0; x < 3; x++)
	{
		std::cout << "--2-- Size of cutting lines , index " << x << " records: " << in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.numberOfRecords() << std::endl;
		// we found a line that had exactly 1 record; generate what we need and then break.
		if (in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.numberOfRecords() == 1)	
		{
			std::cout << "!! Found remaining attempt, building and breaking. " << std::endl;
			auto intersectionRecordsBegin = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.begin();
			TwoDCrawlingAttempt typicalAttempt(TwoDCrawlingType::TYPICAL, x, intersectionRecordsBegin->first, intersectionRecordsBegin->second);
			crawlingAttemptsVector.push_back(typicalAttempt);
			//break;
		}
	}
}

void CuttableTriangle::produceCutLinePoolsFromAttempts(CuttingTriangle* in_cuttingTriangleRef)
{
	std::cout << "+++++++++++++++ Size of attempts: " << crawlingAttemptsVector.size() << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << "++++++++++++++++++++++++++++++++++++++++++ " << std::endl;

	//std::cout << crawlingAttemptsVector.size() << std::endl;
	int attemptsSizeOutput = 3;
	std::cin >> attemptsSizeOutput;

	
	auto attemptsBegin = crawlingAttemptsVector.begin();
	auto attemptsEnd = crawlingAttemptsVector.end();
	int numberOfCancelledAttempts = 0;
	bool wasTypicalUsed = false;
	
	for (; attemptsBegin != attemptsEnd; attemptsBegin++)
	{
		bool wasAttemptMade = false;
		PoolAndDirectionPair currentPair;
		if (attemptsBegin->crawlingType == TwoDCrawlingType::SLICE)
		{
			std::cout << "SLICE crawl type found..." << std::endl;
			//TwoDCrawlingAttempt* crawlAttemptPtr = &(*attemptsBegin);
			currentPair = buildLinesFromSliceAttempt(&(*attemptsBegin), in_cuttingTriangleRef);
			wasAttemptMade = true;
		}
		else if 
		(
			(attemptsBegin->crawlingType == TwoDCrawlingType::TYPICAL)
			&&
			(wasTypicalUsed == false)
		)
		{
			std::cout << "TYPICAL crawl type found..." << std::endl;
			currentPair = buildLinesFromTypicalAttempt(*attemptsBegin, in_cuttingTriangleRef);
			wasAttemptMade = true;
		}

		// run a CutLineWelder, take its produced pool and use it in the CutTriangleGroupBuilder.
		if (currentPair.isAttemptValid == true)
		{

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

			// if it's valid, and typical, set the wasTypicalUsed flag.
			if
			(
				(currentPair.isAttemptValid == true)
				&&
				(attemptsBegin->crawlingType == TwoDCrawlingType::TYPICAL)
			)
			{
				wasTypicalUsed = true;
			}

		}
		else if 
		(
			(currentPair.isAttemptValid == false)
			&&
			(wasAttemptMade == true)
		)
		{
			std::cout << "This tracing attempt has been marked as invalid; it will not be used. " << std::endl;
			numberOfCancelledAttempts++;
			int validWait = 3;
			std::cin >> validWait;

			
			

		}
	}

	if (numberOfCancelledAttempts == crawlingAttemptsVector.size())
	{
		std::cout << "!!! Number of cancelled attempts (" << numberOfCancelledAttempts <<  ") matches attempts vector size. Continue? " << std::endl;
		//int attemptWait;
		//std::cin >> attemptWait;
		int cancelledLooper = 3;

		// insert the original form of the triangle.
		STriangle originalForm(cuttableTriangleLines[0].pointA, cuttableTriangleLines[1].pointA, cuttableTriangleLines[2].pointA);
		outputTriangles[0] = originalForm;
		//while (cancelledLooper == 3)
		//{

		//}
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

	std::cout << "::::::::::::::::::::::::::::::::::::::::::::::: BEGIN, construction of first two Typical attempt lines. " << std::endl;
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

	std::cout << "::> Found cutting line index is: " << in_attempt.cuttingTriangleLineID << std::endl;
	std::cout << "::> Point registry, for this cutting line is: " << std::endl;
	in_cuttingTriangleRef->cuttingLines[in_attempt.cuttingTriangleLineID].printCuttableIntersections();

	std::cout << "::> cutting line point A: " << cuttingLinePointA.x << ", " << cuttingLinePointA.y << ", " << cuttingLinePointA.z << std::endl;
	std::cout << "::> cutting line point B: " << cuttingLinePointB.x << ", " << cuttingLinePointB.y << ", " << cuttingLinePointB.z << std::endl;
	std::cout << "::> cuttable line point A: " << cuttableLinePointA.x << ", " << cuttableLinePointA.y << ", " << cuttableLinePointA.z << std::endl;
	std::cout << "::> cuttable line point B: " << cuttableLinePointB.x << ", " << cuttableLinePointB.y << ", " << cuttableLinePointB.z << std::endl;
	// two quat machines for finding the points: one for the cutting line, one for the cuttable line
	Vec3Result determinedCuttingAttempt = QuatUtils::findPointForDeterminingCyclingDirection(cuttableLinePointA,
																							cuttableLinePointB,
																							cuttingLinePointA,
																							cuttingLinePointB,
																							cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal
																						);
	glm::vec3 determinedCuttingPointToUse = determinedCuttingAttempt.resultPoint;

	std::cout << "::> Cuttable line normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y 
									  << ", " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;
	std::cout << "::> Cutting point to use, determined by cuttable line: " << determinedCuttingPointToUse.x << ", " << determinedCuttingPointToUse.y << ", " << determinedCuttingPointToUse.z << std::endl;

	Vec3Result determinedCuttableAttempt = QuatUtils::findPointForDeterminingCyclingDirection(
																						cuttingLinePointA, 
																						cuttingLinePointB, 
																						cuttableLinePointA, 
																						cuttableLinePointB, 
																						cuttingLineNormal
																					);
	glm::vec3 cuttablePointToUse = determinedCuttableAttempt.resultPoint;
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

	// flag as invalid, if we detected it (either one would cause an invalid run)
	if
	(
		(determinedCuttingAttempt.isResultValid == false)
		||
		(determinedCuttableAttempt.isResultValid == false)
	)
	{
		returnPair.isAttemptValid = false;
	}
	else
	{
		returnPair.isAttemptValid = true;	// otherwise, the runis valid
	}

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
	else
	{
		std::cout << "!!!!! Warning, could not determine CyclingDirection; halting. " << std::endl;
		int haltVal = 3; 
		std::cin >> haltVal;
	}

	returnPair.pairCyclingDirection = cutLineDirection;
	std::cout << "::::::::::::::::::::::::::::::::::::::::::::::: END, construction of first two Typical attempt lines. " << std::endl;

	return returnPair;
}
CuttableTriangle::PoolAndDirectionPair CuttableTriangle::buildLinesFromSliceAttempt(TwoDCrawlingAttempt* in_attempt, CuttingTriangle* in_cuttingTriangleRef)
{
	PoolAndDirectionPair returnPair;

	// First, find the cyling direction. 
// line A point values, from the CuttingLine
	glm::vec3 cuttingLinePointA = in_cuttingTriangleRef->cuttingLines[in_attempt->cuttingTriangleLineID].pointA;
	glm::vec3 cuttingLinePointB = in_cuttingTriangleRef->cuttingLines[in_attempt->cuttingTriangleLineID].pointB;

	// outward facing normal
	glm::vec3 cuttingLineNormal = in_cuttingTriangleRef->cuttingLines[in_attempt->cuttingTriangleLineID].outwardFacingNormal;

	// |||||||||||||||||||||||||||||||||||| BEGIN Slice test for begin intersection

	// common point between both lines:
	glm::vec3 sharedPoint = in_attempt->beginIntersectingPoint;



	
	int idOfLineFoundInCuttingLine = in_attempt->beginIntersectionLineID;

	// line B point values, from the CuttableLine
	glm::vec3 cuttableLinePointA = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointA;
	glm::vec3 cuttableLinePointB = cuttableTriangleLines[idOfLineFoundInCuttingLine].pointB;

	std::cout << "::> (BEGIN) cutting line point A: " << cuttingLinePointA.x << ", " << cuttingLinePointA.y << ", " << cuttingLinePointA.z << std::endl;
	std::cout << "::> (BEGIN) cutting line point B: " << cuttingLinePointB.x << ", " << cuttingLinePointB.y << ", " << cuttingLinePointB.z << std::endl;
	std::cout << "::> (BEGIN) cuttable line point A: " << cuttableLinePointA.x << ", " << cuttableLinePointA.y << ", " << cuttableLinePointA.z << std::endl;
	std::cout << "::> (BEGIN) cuttable line point B: " << cuttableLinePointB.x << ", " << cuttableLinePointB.y << ", " << cuttableLinePointB.z << std::endl;
	std::cout << "::> (BEGIN) Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;
	// two quat machines for finding the points: one for the cutting line, one for the cuttable line

	Vec3Result cuttableResult = QuatUtils::findPointForDeterminingCyclingDirection(
																						cuttingLinePointA, 
																						cuttingLinePointB, 
																						cuttableLinePointA, 
																						cuttableLinePointB, 
																						cuttingLineNormal
																					 );
	glm::vec3 cuttablePointToUse = cuttableResult.resultPoint;

	//std::cout << "::> Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;
	std::cout << "::> Cuttable point to use, determined by cutting line: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;

	// the CuttableLine spawned by the CuttingLine should be:
	// point A = shared point,
	// point B = selected end point from cuttingLineSolver
	// normal = the cuttable line's centroid facing normal.
	CutLine cuttableCutLine(sharedPoint, cuttablePointToUse, cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal);
	std::cout << "(BEGIN) Cuttable cut line stats: " << std::endl;
	std::cout << "(BEGIN) Cuttable, point A: " << sharedPoint.x << ", " << sharedPoint.y << ", " << sharedPoint.z << std::endl;
	std::cout << "(BEGIN) Cuttable, point B: " << cuttablePointToUse.x << ", " << cuttablePointToUse.y << ", " << cuttablePointToUse.z << std::endl;
	std::cout << "(BEGIN) Cuttable, normal: " << cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.x << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.y << ", "
		<< cuttableTriangleLines[idOfLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal.z << std::endl;

	// for a SLICE attempt, the first line is always going to be the sliced line.
	CutLine slicedCutLineBegin(in_attempt->endIntersectingPoint, in_attempt->beginIntersectingPoint, in_cuttingTriangleRef->cuttingLines[in_attempt->cuttingTriangleLineID].outwardFacingNormal);

	// |||||||||||||||||||||||||||||||||||| END Slice test for begin intersection

	// ----------------

	// |||||||||||||||||||||||||||||||||||| BEGIN Slice test for end intersection


	// common point between both lines:
	glm::vec3 sharedPointEnd = in_attempt->endIntersectingPoint;

	// outward facing normal
	int idOfEndLineFoundInCuttingLine = in_attempt->endIntersectionLineID;

	// line B point values, from the CuttableLine
	glm::vec3 endCuttableLinePointA = cuttableTriangleLines[idOfEndLineFoundInCuttingLine].pointA;
	glm::vec3 endCuttableLinePointB = cuttableTriangleLines[idOfEndLineFoundInCuttingLine].pointB;

	std::cout << "::> (END) cutting line point A: " << cuttingLinePointA.x << ", " << cuttingLinePointA.y << ", " << cuttingLinePointA.z << std::endl;
	std::cout << "::> (END) cutting line point B: " << cuttingLinePointB.x << ", " << cuttingLinePointB.y << ", " << cuttingLinePointB.z << std::endl;
	std::cout << "::> (END) cuttable line point A: " << endCuttableLinePointA.x << ", " << endCuttableLinePointA.y << ", " << endCuttableLinePointA.z << std::endl;
	std::cout << "::> (END) cuttable line point B: " << endCuttableLinePointB.x << ", " << endCuttableLinePointB.y << ", " << endCuttableLinePointB.z << std::endl;
	std::cout << "::> (END) Cutting line normal: " << cuttingLineNormal.x << ", " << cuttingLineNormal.y << ", " << cuttingLineNormal.z << std::endl;

	Vec3Result endCuttableResult = QuatUtils::findPointForDeterminingCyclingDirection(
																						cuttingLinePointA,
																						cuttingLinePointB,
																						endCuttableLinePointA,
																						endCuttableLinePointB,
																						cuttingLineNormal
																					);
	glm::vec3 endCuttablePointToUse = endCuttableResult.resultPoint;
	CutLine endingCuttableCutLine(sharedPointEnd, endCuttablePointToUse, cuttableTriangleLines[idOfEndLineFoundInCuttingLine].cuttableTriangleCentroidFacingNormal);
	CutLine endingSlicedCutLineBegin(in_attempt->beginIntersectingPoint, in_attempt->endIntersectingPoint, in_cuttingTriangleRef->cuttingLines[in_attempt->cuttingTriangleLineID].outwardFacingNormal);

	// |||||||||||||||||||||||||||||||||||| END Slice test for begin intersection


	// Use the first line in the slice that is detected as being valid; 
	// If using the second result, be sure to swap the intersection ids, for usage in CutLineWelder::handleSliceRun().
	// Otherwise, it's invalid.
	if (cuttableResult.isResultValid == true)
	{
		std::cout << "BEGIN intersection is valid for SLICE. " << std::endl;
		// the second line will be a line where point A = the begin intersecting point, point B = the cuttablePointToUse, and the normal is the centroid facing normal from the cuttable line.
		returnPair.pairPool.insertLineIntoPool(slicedCutLineBegin);
		returnPair.pairPool.insertLineIntoPool(cuttableCutLine);

		std::cout << "||||||||||||||||| printing CutLinePool, after SLICE attempt build from BEGIN intersecting point: " << std::endl;
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
		returnPair.isAttemptValid = true;
	}
	else if (endCuttableResult.isResultValid == true)
	{
		std::cout << "END intersection is valid for SLICE. " << std::endl;
		returnPair.pairPool.insertLineIntoPool(endingSlicedCutLineBegin);
		returnPair.pairPool.insertLineIntoPool(endingCuttableCutLine);

		std::cout << "||||||||||||||||| printing CutLinePool, after SLICE attempt build from END intersecting point: " << std::endl;
		returnPair.pairPool.printLines();

		// determine the cycling direction value to use, which will be based on the value of cuttablePointToUse.
		CyclingDirection cutLineDirection = CyclingDirection::NOVAL;
		if (endCuttablePointToUse == endCuttableLinePointA)
		{
			std::cout << "!! Slice attempt -- CyclingDirection will be BACKWARD." << std::endl;
			cutLineDirection = CyclingDirection::REVERSE;
		}
		else if (endCuttablePointToUse == endCuttableLinePointB)
		{
			std::cout << "!! Slice attempt -- CyclingDirection will be FORWARD." << std::endl;
			cutLineDirection = CyclingDirection::FORWARD;
		}

		// make sure to swap the begin and intersection line IDs.
		int oldBegin = in_attempt->beginIntersectionLineID;
		int oldEnd = in_attempt->endIntersectionLineID;
		in_attempt->beginIntersectionLineID = oldEnd;
		in_attempt->endIntersectionLineID = oldBegin;

		returnPair.pairCyclingDirection = cutLineDirection;
		returnPair.isAttemptValid = true;
	}
	else
	{
		returnPair.isAttemptValid = false;
	}


	// flag as invalid, if we detected it.
	/*
	if
	(
		(cuttableResult.isResultValid == false)
	)
	{
		returnPair.isAttemptValid = false;
	}
	*/


	/*
	// the second line will be a line where point A = the begin intersecting point, point B = the cuttablePointToUse, and the normal is the centroid facing normal from the cuttable line.
	returnPair.pairPool.insertLineIntoPool(slicedCutLineBegin);
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
	*/
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