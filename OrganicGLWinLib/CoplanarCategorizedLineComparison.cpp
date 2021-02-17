#include "stdafx.h"
#include "CoplanarCategorizedLineProducer.h"

void CoplanarCategorizedLineProducer::performLineComparison()
{
	std::cout << "!!! Performing line comparison..." << std::endl;
	// test the basic line intersection finder class...

	// typical intersect test.
	TwoDLineSegment TwoDLineSegmentA(0, 0, 3, 1);	// a line with point A at 0,0, and point B at 3,1	-- should result in a t of .75
	//TwoDLineSegment TwoDLineSegmentA(3, 1, 0, 0);	// a line with point A at 3,1, and point B at 0,0	-- should result in a t of .25
	TwoDLineSegment TwoDLineSegmentB(3, 0, 1, 2);	// "" ""


	// non-overlap test.
	//TwoDLineSegment TwoDLineSegmentA(0, 0, 2, 0);
	//TwoDLineSegment TwoDLineSegmentB(3, 0, 5, 0);

	// overlap test
	//TwoDLineSegment TwoDLineSegmentA(0, 0, 2, 0);
	//TwoDLineSegment TwoDLineSegmentB(1, 0, 3, 0);

	// parallel test
	//TwoDLineSegment TwoDLineSegmentA(0, 0, 2, 0);
	//TwoDLineSegment TwoDLineSegmentB(3, 2, 5, 2);

	// non-intersect test
	//TwoDLineSegment TwoDLineSegmentA(0, 0, 2, 0);
	//TwoDLineSegment TwoDLineSegmentB(3, 5, 5, 0);
	TwoDLineSegmentIntersectAnalyzer analyzer(TwoDLineSegmentA, TwoDLineSegmentB, IntersectAnalyzerOption::NONE, coplanarCategorizedLineProducerLogLevel);




	// run all the lines of each STriangle in the related SPoly, through each of the border lines of the tracked SPoly.
	// -if there is an intersection, find the endpoint of the related SPoly's line that caused the intersection, that would be within the tracked SPoly's area.
	//  (this would be a IntersectionType::PARTIAL_BOUND)
	// -if there is no intersection, check if the related SPoly's line lies completely within the tracked SPoly (this would be a IntersectionType::NON_BOUND)

	// cycle through each border line in the tracked SPoly:
	/*
	auto trackedBorderLinesBegin = trackedSPolyRef->borderLines.begin();
	auto trackedBorderLinesEnd = trackedSPolyRef->borderLines.end();
	for (; trackedBorderLinesBegin != trackedBorderLinesEnd; trackedBorderLinesBegin++)
	{
		TwoDPoint trackedSegmentA = OrganicGLWinUtils::convertGlmVec3To2D(trackedBorderLinesBegin->second.pointA);
		TwoDPoint trackedSegmentB = OrganicGLWinUtils::convertGlmVec3To2D(trackedBorderLinesBegin->second.pointB);
		TwoDLineSegment trackedSegment(trackedSegmentA, trackedSegmentB);

		std::cout << "!! Current trackedBorderLine points are: A->" << trackedSegmentA.x << ", " << trackedSegmentA.y << " | " 
														  << " B->" << trackedSegmentB.x << ", " << trackedSegmentB.y << std::endl;

		auto relatedSTrianglesBegin = relatedSPolyRef->triangles.begin();
		auto relatedSTrianglesEnd = relatedSPolyRef->triangles.end();
		for (; relatedSTrianglesBegin != relatedSTrianglesEnd; relatedSTrianglesBegin++)
		{
			for (int x = 0; x < 3; x++)
			{
				TwoDPoint currentSTriangleTwoDLineSegmentA = OrganicGLWinUtils::convertGlmVec3To2D(relatedSTrianglesBegin->second.triangleLines[x].pointA);
				TwoDPoint currentSTriangleTwoDLineSegmentB = OrganicGLWinUtils::convertGlmVec3To2D(relatedSTrianglesBegin->second.triangleLines[x].pointB);
				TwoDLineSegment currentSTriangleTwoDLineSegment(currentSTriangleTwoDLineSegmentA, currentSTriangleTwoDLineSegmentB);

				std::cout << "--> Comparing trackedBorderLine against currentSTriangleTwoDLineSegment (index " << x <<") with these points: A-> " << currentSTriangleTwoDLineSegmentA.x << ", " << currentSTriangleTwoDLineSegmentA.y << " | "
																												<< " B-> " << currentSTriangleTwoDLineSegmentB.x << ", " << currentSTriangleTwoDLineSegmentB.y << std::endl;
				TwoDLineSegmentIntersectAnalyzer comparator(trackedSegment, currentSTriangleTwoDLineSegment);
				TwoDLineSegmentJudge judge(comparator.analyzedResult, currentSTriangleTwoDLineSegment, trackedSPolyRef);

				// perform analysis here.
			}
		}
	}
	*/


	// (Step 2: ) produce the co-planar categorized lines -- if any -- that the related SPoly would create in the tracked SPoly.
	//		      We assume that the tracked/related SPolys have all been rotated to Z = 0. (rounding to hundredths may be needed too).
	// (Step 3: ) build the CleaveSequence, if there is one to build.
	// (Step 4: ) perform the fracturing, if it needs to be done; remember, the tracked SPoly's copy (trackedCopy) must be set to 
	//            have MassManipulationMode::DESTRUCTION. Then, when the fracturing is done, acquire the resulting SPolySupergroup's SPoly's STriangles. 

	// All STriangles in the tracked SPoly must be compared against all STriangles in the related SPoly.
	// -the generation of lines should be done from the current Striangle belonging to the related SPoly, as it goes through the current STriangle of the tracked SPoly.
	int hostPolyTriangleCount = trackedSPolyRef->numberOfTriangles;
	int guestPolyTriangleCount = relatedSPolyRef->numberOfTriangles;
	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{
		STriangle* hostTrianglePtr = &trackedSPolyRef->triangles[currentHostPolyTriangle];	// for the host triangle lines.
		//bool isHostParallelToGuestTriangle = false;
		for (int currentGuestTriangleIndex = 0; currentGuestTriangleIndex < guestPolyTriangleCount; currentGuestTriangleIndex++)					// .. to each of poly B's tertiaries...
		{
			STriangle* currentGuestTrianglePtr = &relatedSPolyRef->triangles[currentGuestTriangleIndex];			// for the current triangle we're comparing to
			glm::vec3 currentGuestTriangleCentroid = OrganicGLWinUtils::findTriangleCentroid(currentGuestTrianglePtr->triangleLines[0].pointA,
																							currentGuestTrianglePtr->triangleLines[1].pointA,
																							currentGuestTrianglePtr->triangleLines[2].pointA);
			for (int y = 0; y < 3; y++)		// for looping through guestTrianglePtr's lines
			{
				TwoDPoint currentGuestTriangleTwoDLineSegmentA = OrganicGLWinUtils::convertGlmVec3To2D(currentGuestTrianglePtr->triangleLines[y].pointA);
				TwoDPoint currentGuestTriangleTwoDLineSegmentB = OrganicGLWinUtils::convertGlmVec3To2D(currentGuestTrianglePtr->triangleLines[y].pointB);
				TwoDLineSegment currentGuestTriangleSegment(currentGuestTriangleTwoDLineSegmentA, currentGuestTriangleTwoDLineSegmentB);
				std::cout << ":::::::::::::> Current guest triangle segment: " << currentGuestTriangleTwoDLineSegmentA.x << ", " << currentGuestTriangleTwoDLineSegmentA.y << " | "
																			   << currentGuestTriangleTwoDLineSegmentB.x << ", " << currentGuestTriangleTwoDLineSegmentB.y << std::endl;

				if (currentGuestTrianglePtr->triangleLines[y].isBorderLine == 1)
				{
					for (int z = 0; z < 3; z++)	// for looping through hostTrianglePtr's lines
					{
						TwoDPoint currentHostTriangleTwoDLineSegmentA = OrganicGLWinUtils::convertGlmVec3To2D(hostTrianglePtr->triangleLines[z].pointA);
						TwoDPoint currentHostTriangleTwoDLineSegmentB = OrganicGLWinUtils::convertGlmVec3To2D(hostTrianglePtr->triangleLines[z].pointB);
						TwoDLineSegment currentHostTriangleSegment(currentHostTriangleTwoDLineSegmentA, currentHostTriangleTwoDLineSegmentB);
						std::cout << ":::> Current host segment: " << currentHostTriangleTwoDLineSegmentA.x << ", " << currentHostTriangleTwoDLineSegmentA.y << " | "
							<< currentHostTriangleTwoDLineSegmentB.x << ", " << currentHostTriangleTwoDLineSegmentB.y << std::endl;

						//TwoDLineSegmentIntersectAnalyzer comparator(currentGuestTriangleSegment, currentHostTriangleSegment, IntersectAnalyzerOption::NONE);	// run the comparator.
						TwoDLineSegmentIntersectAnalyzer comparator(currentGuestTriangleSegment, currentHostTriangleSegment, IntersectAnalyzerOption::ROUND_CROSS, coplanarCategorizedLineProducerLogLevel);
						if // do this, as long as the resulting line comparison isn't classified as any of the COLINEAR tpyes (COLINEAR_OVERLAP, COLINEAR_NOOVERLAP)
							(
							(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
								||
								(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECTS_POINT_PRECISE)
								||
								(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NO_INTERSECT)
								||
								(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::PARALLEL)
								)
						{
							// -if the guest segment hit a host segment that was a border line, then TwoDPolyIntersectionType::HIT_BORDERLINE
							// -if the guest segment hit a host segment that was NOT a border line, then TwoDPolyIntersectionType::HIT_NONBORDERLINE
							// ...either way, register the HIT.
							currentGuestTriangleSegment.attemptIntersectionInsert(comparator.analyzedResult, &hostTrianglePtr->triangleLines[z]);
						}

						if  // check whether or not the current guets line segment is colinear to any of the lines in the compared-to host triangle.
							(
							(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::COLINEAR_NOOVERLAP)
								||
								(comparator.analyzedResult.intersectType == TwoDLineSegmentIntersectType::COLINEAR_OVERLAP)
								)
						{
							std::cout << "!!! NOTICE: this line is colinear to another line in the host triangle. No CategorizedLine should exist. " << std::endl;
							currentGuestTriangleSegment.isColinearToAnotherLine = true;
						}

						// judge the currentGuestTriangleSegment; put the result into TwoDLineSegmentMetaTracker container.
					}

					// after the current related line has compared itself to all 3 lines of the current tracked STriangle, analyze the results.
					// -if 1 HIT_BORDER_LINE and 1 HIT_NONBORDERLINE,												      -> PARTIAL_BOUND
					// -if 1 HIT_BORDER_LINE, use the point that lies within the compared-to (tracked STriangle)		  -> PARTIAL_BOUND
					// -if no hits, check if both points of the segment lie within the tracked STriangle; if they do then -> NON_BOUND
					CategorizedLine attemptedCategorizedLine = currentGuestTriangleSegment.attemptCategorizedLineConstruction(currentGuestTriangleCentroid, hostTrianglePtr);
					if (currentGuestTriangleSegment.containsCategorizedLine == true)
					{
						std::cout << "!!! Inserting categorized line into pool..." << std::endl;

						int stopVal = 3;
						std::cin >> stopVal;

						categorizedLinePoolRef->insertLineIntoPool(attemptedCategorizedLine);
					}
				}

			}
			std::cout << "!!! End of comparison of tracked SPoly to guest STriangle having index: " << currentGuestTriangleIndex << std::endl;
		}
		/*
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				// check if the current host line 
			}
		}
		*/

		std::cout << "!!_----> finished comparing to one STriangle in the tracked SPoly; enter number to continue. " << std::endl;
		categorizedLinePoolRef->printLinesInPool();
		int continueVal = 3;
		std::cin >> continueVal;
	}

	// before returning, check for any cases where there are INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR
	categorizedLinePoolRef->solveUnsolvedInterceptPointsPrecise(&trackedSPolyRef->borderLines);

	std::cout << "-----> printing lines, post solve: " << std::endl;
	categorizedLinePoolRef->printLinesInPool();
	int someValYeah = 7;
	std::cin >> someValYeah;

}

TwoDPoint CoplanarCategorizedLineProducer::convertGlmVec3To2D(glm::vec3 in_glmvec3)
{
	TwoDPoint returnPoint(in_glmvec3.x, in_glmvec3.y);
	return returnPoint;
}