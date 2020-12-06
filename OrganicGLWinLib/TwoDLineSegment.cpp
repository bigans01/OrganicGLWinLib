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

void TwoDLineSegment::attemptCategorizedLineConstruction()
{
	// -if 2 HIT_BORDER_LINE,																			  -> A_SLICE
	// -if 1 HIT_BORDER_LINE and 1 HIT_NONBORDERLINE,												      -> PARTIAL_BOUND
	// -if 1 HIT_BORDER_LINE, use the point that lies within the compared-to (tracked STriangle)		  -> PARTIAL_BOUND
	// -if no hits, check if both points of the segment lie within the tracked STriangle; if they do then -> NON_BOUND

	// branch for one HIT_BORDER_LINE
	if (numberOfIntersectedBorderLines == 1)
	{
		if (numberOfIntersectedNonBorderLines == 1)
		{
			// 1 HIT_BORDER_LINE and 1 HIT_NONBORDERLINE, -> PARTIAL_BOUND
		}
		else
		{
			// 1 HIT_BORDER_LINE, but need to determine the other point; find which point in the segment lies within the triangle and then -> PARTIAL_BOUND
		}
	}


	// 2 HIT_BORDER_LINE, -> A_SLICE
	if (numberOfIntersectedBorderLines == 2)
	{
		std::cout << "!! Branch for a A_SLICE categorized line entered. " << std::endl;
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
	}
	/*
	else if 
	(

	)
	*/
}