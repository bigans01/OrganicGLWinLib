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
	TwoDLineSegmentIntersectAnalyzer analyzer(TwoDLineSegmentA, TwoDLineSegmentB);




	// run all the lines of each STriangle in the related SPoly, through each of the border lines of the tracked SPoly.
	// -if there is an intersection, find the endpoint of the related SPoly's line that caused the intersection, that would be within the tracked SPoly's area.
	//  (this would be a IntersectionType::PARTIAL_BOUND)
	// -if there is no intersection, check if the related SPoly's line lies completely within the tracked SPoly (this would be a IntersectionType::NON_BOUND)

	// cycle through each border line in the tracked SPoly:
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

				std::cout << "--> Comparing trackedBorderLine against currentSTriangleTwoDLineSegment with these points: A-> " << currentSTriangleTwoDLineSegmentA.x << ", " << currentSTriangleTwoDLineSegmentA.y << " | "
																												<< " B-> " << currentSTriangleTwoDLineSegmentB.x << ", " << currentSTriangleTwoDLineSegmentB.y << std::endl;
				TwoDLineSegmentIntersectAnalyzer comparator(trackedSegment, currentSTriangleTwoDLineSegment);
				TwoDLineSegmentJudge judge(comparator.analyzedResult, currentSTriangleTwoDLineSegment, trackedSPolyRef);

				// perform analysis here.
			}
		}
	}

	/*
	std::cout << "############# Testing function for if point is within triangle. " << std::endl;
	glm::vec3 pointToCheck;
	pointToCheck.x = .5;
	pointToCheck.y = .5;

	glm::vec3 trianglePoint1;
	glm::vec3 trianglePoint2;
	trianglePoint2.y = 1.0f;
	glm::vec3 trianglePoint3;
	trianglePoint3.x = 1.0f;

	OrganicGLWinUtils::checkIfPointLiesWithinTriangle(pointToCheck, trianglePoint1, trianglePoint2, trianglePoint3);
	*/
}

TwoDPoint CoplanarCategorizedLineProducer::convertGlmVec3To2D(glm::vec3 in_glmvec3)
{
	TwoDPoint returnPoint(in_glmvec3.x, in_glmvec3.y);
	return returnPoint;
}