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
	}
	else if		// there was no intersection, and it wasn't COLINEAR
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

void TwoDLineSegment::attemptCategorizedLineConstruction(glm::vec3 in_guestTriangleCentroid, STriangle* in_hostSTrianglePtr)
{
	// -if 2 HIT_BORDER_LINE,																			  -> A_SLICE
	// -if 1 HIT_BORDER_LINE and 1 HIT_NONBORDERLINE,												      -> PARTIAL_BOUND
	// -if 1 HIT_BORDER_LINE, use the point that lies within the compared-to (tracked STriangle)		  -> PARTIAL_BOUND
	// -if no hits, check if both points of the segment lie within the tracked STriangle; if they do then -> NON_BOUND

	if (intersectionRecords.size() == 1)
	{

	}

	else if (intersectionRecords.size() == 2)
	{
		// 1 HIT_BORDER_LINE, 1 HIT_NONBORDERLINE
		if (numberOfIntersectedBorderLines == 1)	// branch for one HIT_BORDER_LINE
		{

		}


		// 2 HIT_BORDER_LINE, -> A_SLICE
		if (numberOfIntersectedBorderLines == 2)
		{
			std::cout << "!! Branch for a A_SLICE categorized line entered. " << std::endl;
			containsCategorizedLine = true;
			CategorizedLine newLine;
			auto recordsBegin = intersectionRecords.begin();
			newLine.line.pointA = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
			newLine.line.isPointAOnBorder = 1;
			newLine.line.pointABorder = recordsBegin->intersectedBorderLineID;

			recordsBegin++;
			newLine.line.pointB = OrganicGLWinUtils::convert2DToGlmVec3(recordsBegin->intersectedPoint);
			newLine.line.isPointBOnBorder = 1;
			newLine.line.pointBBorder = recordsBegin->intersectedBorderLineID;

			newLine.emptyNormal = determineCoplanarCategorizedLineEmptyNormal(in_guestTriangleCentroid, newLine.line.pointA, newLine.line.pointB);
			newLine.type = IntersectionType::A_SLICE;

			std::cout << "!! Newly formed CategorizedLine stats are: " << std::endl;
			std::cout << "Point A: " << newLine.line.pointA.x << ", " << newLine.line.pointA.y << ", " << newLine.line.pointA.z << std::endl;
			std::cout << "isPointAOnBorder: " << newLine.line.isPointAOnBorder << std::endl;
			std::cout << "pointABorder" << newLine.line.pointABorder << std::endl;
			std::cout << "Point B: " << newLine.line.pointB.x << ", " << newLine.line.pointB.y << ", " << newLine.line.pointB.z << std::endl;
			std::cout << "isPointBOnBorder: " << newLine.line.isPointBOnBorder << std::endl;
			std::cout << "pointBBorder" << newLine.line.pointBBorder << std::endl;

			
		}
	}

	else if (intersectionRecords.size() == 3)
	{

	}

	// No hits -- check for -> NON_BOUND
	else if
	(
		(numberOfIntersectedBorderLines == 0)
		&&
		(numberOfIntersectedNonBorderLines == 0)
	)
	{
		std::cout << "!! Branch for potential NON_BOUND entered. " << std::endl;
		glm::vec3 convertedPointA = OrganicGLWinUtils::convert2DToGlmVec3(a);
		glm::vec3 convertedPointB = OrganicGLWinUtils::convert2DToGlmVec3(b);
		if
		(	
			(OrganicGLWinUtils::checkIfPointLiesWithinTriangle(convertedPointA, in_hostSTrianglePtr->triangleLines[0].pointA, in_hostSTrianglePtr->triangleLines[1].pointA, in_hostSTrianglePtr->triangleLines[2].pointA) == true)
			&&
			(OrganicGLWinUtils::checkIfPointLiesWithinTriangle(convertedPointB, in_hostSTrianglePtr->triangleLines[0].pointA, in_hostSTrianglePtr->triangleLines[1].pointA, in_hostSTrianglePtr->triangleLines[2].pointA) == true)
		)
		{
			std::cout << "!! this segment is a NON_BOUND that exists within the area of the compared-to STriangle. " << std::endl;
			containsCategorizedLine = true;
		}
		else
		{
			std::cout << "!! this segment doesn't lie within the compared-to STriangle; no CategorizedLine exists! " << std::endl;
		}
	}
	/*
	else if 
	(

	)
	*/
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

	quatPoints.pointsRefVector.push_back(&segmentACopy);
	quatPoints.pointsRefVector.push_back(&segmentBCopy);
	quatPoints.pointsRefVector.push_back(&centroidCopy);

	pointTranslator.performCheck(quatPoints.getFirstPoint());
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// after translation, insert a reference to the calculatedEmptyNormal
	quatPoints.pointsRefVector.push_back(&calculatedEmptyNormal);

	rotationManager.initializeAndRunForCoplanarCategorizedLineEmptyNormal(&quatPoints);

	std::cout << "!!!!! --> Calculated empty normal is: " << calculatedEmptyNormal.x << ", " << calculatedEmptyNormal.y << ",  " << calculatedEmptyNormal.z << std::endl;
	


	return calculatedEmptyNormal;
}