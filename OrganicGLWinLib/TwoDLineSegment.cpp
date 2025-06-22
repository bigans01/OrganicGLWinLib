#include "stdafx.h"
#include "TwoDLineSegment.h"

void TwoDLineSegment::attemptIntersectionInsert(TwoDLineSegmentIntersectResult in_result, STriangleLine* in_hostTriangleLineRef)
{
	// -if the guest segment hit a host segment that was a border line, then TwoDPolyIntersectionType::HIT_BORDERLINE
	if			// inersection of a border-line
	(
		(in_hostTriangleLineRef->isBorderLine == 1)
		&&
		(in_result.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
	)
	{
		numberOfIntersectedBorderLines++;
		std::cout << "----> border line hit; ID of border line that was hit: " << int(in_hostTriangleLineRef->borderLineID) << std::endl;
		std::cout << "----> point was: " << in_result.intersectedPoint.x << ", " << in_result.intersectedPoint.y << std::endl;
		insertIntersectionRecord(TwoDSPolyIntersectionType::HIT_BORDERLINE, true, in_hostTriangleLineRef->borderLineID, in_result.intersectedPoint);
		containsIntercepts = true;
	}
	else if     // intersection of a non-border line
	(
		(in_hostTriangleLineRef->isBorderLine == 0)
		&&
		(in_result.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
	)
	{
		numberOfIntersectedNonBorderLines++;
		insertIntersectionRecord(TwoDSPolyIntersectionType::HIT_NONBORDERLINE, false, 0, in_result.intersectedPoint);
		containsIntercepts = true;
	}
	
	else if
	(
		(in_result.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECTS_POINT_PRECISE)
	)
	{
		//numberOfInterceptsBorderLinePointPrecise++;
		bool wasHitOnBorderLine = false;
		TwoDSPolyIntersectionType currentType = TwoDSPolyIntersectionType::PRECISE_HIT_NONBORDERLINE;
		unsigned char borderLineID = 0;
		if (in_hostTriangleLineRef->isBorderLine == 1)
		{
			numberOfInterceptsBorderLinePointPrecise++;
			bool wasHitOnBorderLine = true;
			currentType = TwoDSPolyIntersectionType::PRECISE_HIT_BORDERLINE;
			borderLineID = in_hostTriangleLineRef->borderLineID;
		}
		else if (in_hostTriangleLineRef->isBorderLine == 0)
		{
			numberOfInterceptsNonBorderLinePointPrecise++;
		}
		insertIntersectionRecord(currentType, wasHitOnBorderLine, borderLineID, in_result.intersectedPoint);
		containsIntercepts = true;

	}
	else if		// there was no intersection, and it wasn't COLINEAR; don't set containsIntercepts flag
	(
		(in_result.intersectType == TwoDLineSegmentIntersectType::NO_INTERSECT)
		||
		(in_result.intersectType == TwoDLineSegmentIntersectType::PARALLEL)
	)
	{
		insertIntersectionRecord(TwoDSPolyIntersectionType::NO_HIT, false, 0, in_result.intersectedPoint);
	}
	// -if the guest segment hit a host segment that was NOT a border line, then TwoDPolyIntersectionType::HIT_NONBORDERLINE
	// ...either way, register the HIT.
}

void TwoDLineSegment::insertIntersectionRecord(TwoDSPolyIntersectionType in_intersectionType, bool in_isOnBorderLine, unsigned char in_intersectedBorderLineID, TwoDPoint in_intersectedPoint)
{
	TwoDSPolyIntersectionRecord newRecord(in_intersectionType, in_isOnBorderLine, in_intersectedBorderLineID, in_intersectedPoint);
	intersectionRecords.push_back(newRecord);
}

CategorizedLine TwoDLineSegment::attemptCategorizedLineConstruction(glm::vec3 in_guestTriangleCentroid, STriangle* in_hostSTrianglePtr)
{
	// -if 2 HIT_BORDER_LINE,																			  -> A_SLICE
	// -if 1 HIT_BORDER_LINE and 1 HIT_NONBORDERLINE,												      -> PARTIAL_BOUND
	// -if 1 HIT_BORDER_LINE, use the point that lies within the compared-to (tracked STriangle)		  -> PARTIAL_BOUND
	// -if no hits, check if both points of the segment lie within the tracked STriangle; if they do then -> NON_BOUND

	std::cout << "########### Attempting construction; stats are: " << std::endl;
	std::cout << "intersected border line count: " << numberOfIntersectedBorderLines << std::endl;
	std::cout << "intersected nonborder line count: " << numberOfIntersectedNonBorderLines << std::endl;
	std::cout << "intersected border lines precise count: " << numberOfInterceptsBorderLinePointPrecise << std::endl;
	std::cout << "intersected non border lines precise count: " << numberOfInterceptsNonBorderLinePointPrecise << std::endl;

	CategorizedLine newLine;
	if (isColinearToAnotherLine == false)
	{

		if (containsIntercepts == true)
		{
			if (intersectionRecords.size() == 1)
			{
				// 1 HIT_BORDERLINE --OR-- 1 HIT_NONBORDERLINE
				if 
				(
					(numberOfIntersectedBorderLines == 1)	// branch for one HIT_BORDER_LINE
					||
					(numberOfIntersectedNonBorderLines ==1)
				)
				{

					std::cout << "!! Branch for a PARTIAL_BOUND categorized line entered. " << std::endl;
					if (numberOfIntersectedBorderLines == 1)
					{
						newLine.type = IntersectionType::PARTIAL_BOUND;
						std::cout << "Record to analyze is a HIT_BORDERLINE. " << std::endl;
					}
					else if (numberOfIntersectedNonBorderLines == 1)
					{
						newLine.type = IntersectionType::NON_BOUND;
						std::cout << "Record to analyze is a HIT_NONBORDERLINE. " << std::endl;
					}

					containsCategorizedLine = true;

					auto recordsBegin = intersectionRecords.begin();
					newLine.line.pointA = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
					newLine.line.isPointAOnBorder = 1;
					newLine.line.pointABorder = recordsBegin->intersectedBorderLineID;

					// check which point lies within the triangle; check point A. if it isn't point A, then it must be point B.
					glm::vec3 convertedSegmentPointA = OrganicGLWinUtils::convert2DToGlmVec3(a);
					glm::vec3 convertedSegmentPointB = OrganicGLWinUtils::convert2DToGlmVec3(b);
					if
						(

							QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedSegmentPointA,
								in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version()
							) == true
						)
					{
						std::cout << "!! The 2d point, A, lies within the triangle: " << convertedSegmentPointA.x << ", " << convertedSegmentPointA.y << ", " << convertedSegmentPointA.z << std::endl;
						newLine.line.pointB = convertedSegmentPointA;
					}
					else if
						(
					

							QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedSegmentPointB,
								in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version()) == true
							)
					{
						std::cout << "!! The 2d point, B, lies within the triangle: " << convertedSegmentPointB.x << ", " << convertedSegmentPointB.y << ", " << convertedSegmentPointB.z << std::endl;
						newLine.line.pointB = convertedSegmentPointB;
					}
					else
					{
						std::cout << "!! NOTICE: although this line hit a border line, neither segment's point is within the triangle. this is an invalid CategorizedLine." << std::endl;
						int someVal = 3;
						std::cin >> someVal;
						containsCategorizedLine = false;
					}
					newLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, newLine.line.pointA, newLine.line.pointB);
					newLine.line.numberOfBorderLines = 1;
					//newLine.type = IntersectionType::PARTIAL_BOUND;
					std::cout << "!! Newly formed CategorizedLine stats are: " << std::endl;
					std::cout << "Point A: " << newLine.line.pointA.x << ", " << newLine.line.pointA.y << ", " << newLine.line.pointA.z << std::endl;
					std::cout << "isPointAOnBorder: " << newLine.line.isPointAOnBorder << std::endl;
					std::cout << "pointABorder" << newLine.line.pointABorder << std::endl;
					std::cout << "Point B: " << newLine.line.pointB.x << ", " << newLine.line.pointB.y << ", " << newLine.line.pointB.z << std::endl;
					std::cout << "isPointBOnBorder: " << newLine.line.isPointBOnBorder << std::endl;
					std::cout << "pointBBorder" << newLine.line.pointBBorder << std::endl;
				}

				// 1 HIT NON_BORDER_LINE
				//else if (numberOfIntersectedNonBorderLines == 1)
				//{
					//std::cout << "!! Branch for analyzing a single non border line record entered." << std::endl;
				//}

			}

			else if (intersectionRecords.size() == 2)
			{
				// 1 HIT_BORDER_LINE, 1 HIT_NONBORDERLINE

				// the two points shouldn't be the same, if the line is valid.
				if (checkIf2dPointsMatch(intersectionRecords.begin()->intersectedPoint, intersectionRecords.rbegin()->intersectedPoint) == false)
				{
					std::cout << "!! Passed 2d point match check..." << std::endl;

					if (numberOfIntersectedBorderLines == 1)	// branch for one HIT_BORDER_LINE
					{

						std::cout << "!! Branch for a PARTIAL_BOUND categorized line entered. " << std::endl;
						containsCategorizedLine = true;
						//CategorizedLine newLine;
						// one record should be HIT_BORDERLINE, one should be HIT_NONBORDERLINE
						auto recordsBegin = intersectionRecords.begin();
						auto recordsEnd = intersectionRecords.end();
						for (; recordsBegin != recordsEnd; recordsBegin++)
						{
							if (recordsBegin->intersectionType == TwoDSPolyIntersectionType::HIT_BORDERLINE)
							{
								newLine.line.pointA = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
								newLine.line.isPointAOnBorder = 1;
								newLine.line.pointABorder = recordsBegin->intersectedBorderLineID;
							}
							else if (recordsBegin->intersectionType == TwoDSPolyIntersectionType::HIT_NONBORDERLINE)
							{
								newLine.line.pointB = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
							}
						}
						newLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, newLine.line.pointA, newLine.line.pointB);
						newLine.line.numberOfBorderLines = 1;
						newLine.type = IntersectionType::PARTIAL_BOUND;

					}


					// 2 HIT_BORDER_LINE, -> A_SLICE
					else if (numberOfIntersectedBorderLines == 2)
					{
						// check that the border line points are not the same; if they are, this is an invalid CategorizedLine.

						std::cout << "!! Branch for a A_SLICE categorized line entered. " << std::endl;
						containsCategorizedLine = true;
						//CategorizedLine newLine;
						auto recordsBegin = intersectionRecords.begin();
						newLine.line.pointA = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
						newLine.line.isPointAOnBorder = 1;
						newLine.line.pointABorder = recordsBegin->intersectedBorderLineID;

						recordsBegin++;
						newLine.line.pointB = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
						newLine.line.isPointBOnBorder = 1;
						newLine.line.pointBBorder = recordsBegin->intersectedBorderLineID;

						newLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, newLine.line.pointA, newLine.line.pointB);
						newLine.line.numberOfBorderLines = 2;
						newLine.type = IntersectionType::A_SLICE;

						std::cout << "!! Newly formed CategorizedLine stats are: " << std::endl;
						std::cout << "Point A: " << newLine.line.pointA.x << ", " << newLine.line.pointA.y << ", " << newLine.line.pointA.z << std::endl;
						std::cout << "isPointAOnBorder: " << newLine.line.isPointAOnBorder << std::endl;
						std::cout << "pointABorder" << newLine.line.pointABorder << std::endl;
						std::cout << "Point B: " << newLine.line.pointB.x << ", " << newLine.line.pointB.y << ", " << newLine.line.pointB.z << std::endl;
						std::cout << "isPointBOnBorder: " << newLine.line.isPointBOnBorder << std::endl;
						std::cout << "pointBBorder" << newLine.line.pointBBorder << std::endl;


					}
					else if (numberOfInterceptsBorderLinePointPrecise == 2)
					{
						std::cout << "!!! Logic for interceptsPointPrecise x 2 entered..." << std::endl;
						glm::vec3 convertedPointA = OrganicGLWinUtils::convert2DToGlmVec3(a);
						glm::vec3 convertedPointB = OrganicGLWinUtils::convert2DToGlmVec3(b);

						std::cout << "Converted point A is: " << convertedPointA.x << ", " << convertedPointA.y << ", " << convertedPointA.z << ", " << std::endl;
						std::cout << "Converted point B is: " << convertedPointB.x << ", " << convertedPointB.y << ", " << convertedPointB.z << ", " << std::endl;

						if
							(
								(QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedPointA, 
																			in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
																			in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(), 
																			in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version()) 
									                                        == true)
								&&
			
								(QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedPointB, 
																			in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(), 
																			in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(), 
																			in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version()) 
																			== true)
							)
						{
							std::cout << "!! this segment is a INTERCEPTS_POINT_PRECISE that exists within the area of the compared-to STriangle. " << std::endl;
							std::cout << "!! Intersection records size was: " << intersectionRecords.size() << std::endl;

							containsCategorizedLine = true;
						}
						else
						{
							std::cout << "!! this segment doesn't lie within the compared-to STriangle; no CategorizedLine exists! " << std::endl;
						}
					}
				}
				else    // The points do Match, so the CategorizedLine is invalid. 
						// Remember, if the numberOfInteceptsNonBorderLinePointPrecise == 1, then the categorized line we send back will be of the type,
						// IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR. We will then need to find the two border lines of this intercept later,
						// and then convert the appropriate data into a categorized line of IntersectionType::INTERCEPTS_POINT_PRECISE, but only at the very end, when 
						// duplicate CategorizedLines in the pool have been purged. Once purging is complete, loop through the list of lines and look for any that are
						// IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR, and determine the border lines required for INTERCEPTS_BORDER_LINE_PRECISE.
				{
					std::cout << "!!! NOTICE: points match, halting. " << std::endl;
					if
					(
						(numberOfInterceptsBorderLinePointPrecise == 2)
						||
						(
							(numberOfInterceptsBorderLinePointPrecise == 1)
							&&
							(numberOfInterceptsNonBorderLinePointPrecise == 1)
						)
					)
					{
						std::cout << "!!! >> point match, but with interceptPointsPrecise as having a count of 2. " << std::endl;
						TwoDPoint pointToCheckIfExistsWithinRelatedArea;
						auto recordsBegin = intersectionRecords.begin();

						std::cout << "!! intersected point: " << recordsBegin->intersectedPoint.x << ", " << recordsBegin->intersectedPoint.y << std::endl;
						std::cout << "!! a: " << a.x << ", " << a.y << std::endl;
						std::cout << "!! b: " << b.x << ", " << b.y << std::endl;

						bool matchedFlag = false;
						if
						(
							(recordsBegin->intersectedPoint.x == a.x)
							&&
							(recordsBegin->intersectedPoint.y == a.y)
						)
						{
							std::cout << "Matched point is the equivalent of pointA in this segment; point to check whether or not it exists within the triangle will be pointB of the segment. " << std::endl;
							pointToCheckIfExistsWithinRelatedArea = b;
							matchedFlag = true;
						}
						else if
						(
							(recordsBegin->intersectedPoint.x == b.x)
							&&
							(recordsBegin->intersectedPoint.y == b.y)
						)
						{
							std::cout << "Matched point is the equivalent of pointB in this segment; point to check whether or not it exists within the triangle will be pointB of the segment. " << std::endl;
							pointToCheckIfExistsWithinRelatedArea = a;
							matchedFlag = true;
						}


						glm::vec3 convertedPoint = OrganicGLWinUtils::convert2DToGlmVec3(pointToCheckIfExistsWithinRelatedArea);
						if
						(

							(QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedPoint,
								in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(),
								in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version())
								== true)

							&&
							(matchedFlag == true)
						)
						{
							std::cout << "Point to check lies within this triangle; can return a categorized line." << std::endl;
							containsCategorizedLine = true;
							newLine = produceAppropriateInterceptsPreciseCategorizedLineFromTwoRecords(convertedPoint, in_guestTriangleCentroid);

						}
						else
						{
							std::cout << "Point to check DOES NOT lie within triangle." << std::endl;
						}
					}
					//int someVal = 3;
					//std::cin >> someVal;
				}
			}

			else if (intersectionRecords.size() == 3)
			{
				std::cout << "!!! ### Branch for INTERCEPTS_POINT_PRECISE categorized line entered..." << std::endl;

				// just for testing, print the points in each record.
				auto intersectionRecordsBegin = intersectionRecords.begin();
				auto intersectionRecordsEnd = intersectionRecords.end();
				for (; intersectionRecordsBegin != intersectionRecordsEnd; intersectionRecordsBegin++)
				{
					std::cout << "Record point is: " << intersectionRecordsBegin->intersectedPoint.x << ", " << intersectionRecordsBegin->intersectedPoint.y << std::endl;
				}

				containsCategorizedLine = true;
				newLine = produceAppropriateInterceptsPreciseCategorizedLineFromThreeRecords(in_guestTriangleCentroid);
				

				// remember: point A of an intercepts point precise should be on the border; point B should be somewhere within the triangle
				// Step 1.) find point B.

			}
		}
		// No hits -- check for -> NON_BOUND
		else if
		(
			/*
			(numberOfIntersectedBorderLines == 0)
			&&
			(numberOfIntersectedNonBorderLines == 0)
			&&
			(numberOfInterceptsBorderLinePointPrecise == 0)
			&&
			(numberOfInterceptsNonBorderLinePointPrecise == 0)
			*/
			containsIntercepts == false
		)
		{
			std::cout << "!! Branch for potential NON_BOUND entered. " << std::endl;
			glm::vec3 convertedPointA = OrganicGLWinUtils::convert2DToGlmVec3(a);
			glm::vec3 convertedPointB = OrganicGLWinUtils::convert2DToGlmVec3(b);

			std::cout << "Converted point A is: " << convertedPointA.x << ", " << convertedPointA.y << ", " << convertedPointA.z << ", " << std::endl;
			std::cout << "Converted point B is: " << convertedPointB.x << ", " << convertedPointB.y << ", " << convertedPointB.z << ", " << std::endl;
			
			if
				(

					(QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedPointA,
																in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
																in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(),
																in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version())
																== true)

					&&

					(QuatUtils::checkIfPointLiesWithinTrianglePBZ(convertedPointB,
																in_hostSTrianglePtr->triangleLines[0].fetchPointAGlmVec3Version(),
																in_hostSTrianglePtr->triangleLines[1].fetchPointAGlmVec3Version(),
																in_hostSTrianglePtr->triangleLines[2].fetchPointAGlmVec3Version())
																== true)

					)
			{
				std::cout << "!! this segment is a NON_BOUND that exists within the area of the compared-to STriangle. " << std::endl;
				std::cout << "!! Intersection records size was: " << intersectionRecords.size() << std::endl;

				containsCategorizedLine = true;
			}
			else
			{
				std::cout << "!! this segment doesn't lie within the compared-to STriangle; no CategorizedLine exists! " << std::endl;
			}
		}
	}
	else
	{
		std::cout << "NOTICE: !! Line was detected as colinear to another line; no categorized line exists. " << std::endl;
	}
	return newLine;
}

glm::vec3 TwoDLineSegment::determineCoplanarCategorizedLineEmptyNormal(glm::vec3 in_guestTriangleCentroid, glm::vec3 in_pointA, glm::vec3 in_pointB)
{
	glm::vec3 calculatedEmptyNormal;
	glm::vec3 segmentACopy = in_pointA;
	glm::vec3 segmentBCopy = in_pointB;
	glm::vec3 centroidCopy = in_guestTriangleCentroid;

	QuatRotationPoints quatPoints;
	QuatRotationManager rotationManager;
	PointTranslationCheck pointTranslator;

	quatPoints.insertPointRefs(&segmentACopy, &segmentBCopy, &centroidCopy);

	pointTranslator.performCheck(quatPoints.getFirstPoint());
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// after translation, insert a reference to the calculatedEmptyNormal
	quatPoints.insertPointRefs(&calculatedEmptyNormal);

	rotationManager.initializeAndRunForCoplanarCategorizedLineEmptyNormal(&quatPoints);

	std::cout << "!!!!! --> Calculated empty normal is: " << calculatedEmptyNormal.x << ", " << calculatedEmptyNormal.y << ",  " << calculatedEmptyNormal.z << std::endl;
	


	return calculatedEmptyNormal;
}

bool TwoDLineSegment::checkIf2dPointsMatch(TwoDPoint in_pointA, TwoDPoint in_pointB)
{
	bool matchFound = false;
	if
	(
		(in_pointA.x == in_pointB.x)
		&&
		(in_pointA.y == in_pointB.y)
	)
	{
		std::cout << "NOTICE: 2d points match!" << std::endl;
		matchFound = true;
	}
	return matchFound;
}

CategorizedLine TwoDLineSegment::produceAppropriateInterceptsPreciseCategorizedLineFromThreeRecords(glm::vec3 in_guestTriangleCentroid)
{
	CategorizedLine returnLine;
	if (numberOfInterceptsBorderLinePointPrecise == 2)
	{
		

		short borderMeta[2];			// array to store the border ids
		glm::vec3 pointA;				// the point that is on the border between the two lines
		glm::vec3 pointB;
		int borderIDCounter = 0;
		auto intersectionRecordsBegin = intersectionRecords.begin();
		auto intersectionRecordsEnd = intersectionRecords.end();
		for (; intersectionRecordsBegin != intersectionRecordsEnd; intersectionRecordsBegin++)
		{
			if (intersectionRecordsBegin->intersectionType == TwoDSPolyIntersectionType::PRECISE_HIT_BORDERLINE)
			{
				std::cout << "--> (Three records) Found border line ID to insert: " << intersectionRecordsBegin->intersectedBorderLineID << std::endl;
				borderMeta[borderIDCounter++] = short(intersectionRecordsBegin->intersectedBorderLineID);
				pointA = OrganicGLWinUtils::convert2DToGlmVec3(intersectionRecordsBegin->intersectedPoint);	// this would be done twice, but always same value
			}
			else
			{
				std::cout << "--> Found point B " << intersectionRecordsBegin->intersectedPoint.x << ", " << intersectionRecordsBegin->intersectedPoint.y << std::endl;
				pointB = OrganicGLWinUtils::convert2DToGlmVec3(intersectionRecordsBegin->intersectedPoint);
			}
		}

		returnLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
		returnLine.line.pointA = pointA;
		returnLine.line.pointB = pointB;
		returnLine.line.numberOfBorderLines = 1;
		returnLine.line.pointABorder = borderMeta[0];
		returnLine.line.pointBBorder = borderMeta[1];
		returnLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, returnLine.line.pointA, returnLine.line.pointB);

		std::cout << "!! Produced line stats are: " << std::endl;
		std::cout << "! Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
		std::cout << "! Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
		std::cout << "! point A border: " << returnLine.line.pointABorder << std::endl;
		std::cout << "! point B border: " << returnLine.line.pointBBorder << std::endl;
		std::cout << "! Empty normal: " << returnLine.emptyNormal.x << ", " << returnLine.emptyNormal.y << ", " << returnLine.emptyNormal.z << std::endl;
	}
	else if
	(
		(numberOfInterceptsBorderLinePointPrecise == 1)
		&&
		(numberOfInterceptsNonBorderLinePointPrecise == 1)
	)
	{

	}

	return returnLine;
}

CategorizedLine TwoDLineSegment::produceAppropriateInterceptsPreciseCategorizedLineFromTwoRecords(glm::vec3 in_newLinePointB, glm::vec3 in_guestTriangleCentroid)
{
	CategorizedLine returnLine;
	if (numberOfInterceptsBorderLinePointPrecise == 2)
	{
		short borderMeta[2];			// array to store the border ids
		glm::vec3 pointA;				// the point that is on the border between the two lines
		glm::vec3 pointB = in_newLinePointB;
		int borderIDCounter = 0;
		auto intersectionRecordsBegin = intersectionRecords.begin();
		auto intersectionRecordsEnd = intersectionRecords.end();
		for (; intersectionRecordsBegin != intersectionRecordsEnd; intersectionRecordsBegin++)
		{
			if (intersectionRecordsBegin->intersectionType == TwoDSPolyIntersectionType::PRECISE_HIT_BORDERLINE)
			{
				std::cout << "--> (Two-records, 2 precise border points) Found border line ID to insert: " << intersectionRecordsBegin->intersectedBorderLineID << std::endl;
				borderMeta[borderIDCounter++] = short(intersectionRecordsBegin->intersectedBorderLineID);
				pointA = OrganicGLWinUtils::convert2DToGlmVec3(intersectionRecordsBegin->intersectedPoint);	// this would be done twice, but always same value
			}
		}

		returnLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
		returnLine.line.pointA = pointA;
		returnLine.line.pointB = pointB;
		returnLine.line.numberOfBorderLines = 1;
		returnLine.line.pointABorder = borderMeta[0];
		returnLine.line.pointBBorder = borderMeta[1];
		returnLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, returnLine.line.pointA, returnLine.line.pointB);
	}
	else if
	(
		(numberOfInterceptsBorderLinePointPrecise == 1)
		&&
		(numberOfInterceptsNonBorderLinePointPrecise == 1)
	)
	{
		glm::vec3 pointA;				// the point that is on the border between the two lines
		glm::vec3 pointB = in_newLinePointB;
		auto intersectionRecordsBegin = intersectionRecords.begin();
		auto intersectionRecordsEnd = intersectionRecords.end();
		for (; intersectionRecordsBegin != intersectionRecordsEnd; intersectionRecordsBegin++)
		{
			if (intersectionRecordsBegin->intersectionType == TwoDSPolyIntersectionType::PRECISE_HIT_NONBORDERLINE)		// find the point in the nonborder line record
			{
				std::cout << "--> (Two-records, 1/1 split), found point: " << intersectionRecordsBegin->intersectedPoint.x << ", " << intersectionRecordsBegin->intersectedPoint.y << std::endl;
				pointA = OrganicGLWinUtils::convert2DToGlmVec3(intersectionRecordsBegin->intersectedPoint);
			}
		}
		returnLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR;
		returnLine.line.pointA = pointA;
		returnLine.line.pointB = pointB;
		returnLine.line.numberOfBorderLines = 1;
		returnLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, returnLine.line.pointA, returnLine.line.pointB);

		std::cout << "!! Produced line stats are: " << std::endl;
		std::cout << "! Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
		std::cout << "! Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
		std::cout << "! Empty normal: " << returnLine.emptyNormal.x << ", " << returnLine.emptyNormal.y << ", " << returnLine.emptyNormal.z << std::endl;
	}
	return returnLine;
}