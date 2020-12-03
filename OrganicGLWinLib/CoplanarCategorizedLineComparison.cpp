#include "stdafx.h"
#include "CoplanarCategorizedLineProducer.h"

void CoplanarCategorizedLineProducer::performLineComparison()
{
	std::cout << "!!! Performing line comparison..." << std::endl;
	// test the basic line intersection finder class...

	// typical intersect test.
	LineSegment lineSegmentA(0, 0, 3, 1);	// a line with point A at 0,0, and point B at 3,1	-- should result in a t of .75
	//LineSegment lineSegmentA(3, 1, 0, 0);	// a line with point A at 3,1, and point B at 0,0	-- should result in a t of .25
	LineSegment lineSegmentB(3, 0, 1, 2);	// "" ""


	// non-overlap test.
	//LineSegment lineSegmentA(0, 0, 2, 0);
	//LineSegment lineSegmentB(3, 0, 5, 0);

	// overlap test
	//LineSegment lineSegmentA(0, 0, 2, 0);
	//LineSegment lineSegmentB(1, 0, 3, 0);

	// parallel test
	//LineSegment lineSegmentA(0, 0, 2, 0);
	//LineSegment lineSegmentB(3, 2, 5, 2);

	// non-intersect test
	//LineSegment lineSegmentA(0, 0, 2, 0);
	//LineSegment lineSegmentB(3, 5, 5, 0);
	LineSegmentIntersectAnalyzer analyzer(lineSegmentA, lineSegmentB);




	// run all the lines of each STriangle in the related SPoly, through each of the border lines of the tracked SPoly.
	// -if there is an intersection, find the endpoint of the related SPoly's line that caused the intersection, that would be within the tracked SPoly's area.
	//  (this would be a IntersectionType::PARTIAL_BOUND)
	// -if there is no intersection, check if the related SPoly's line lies completely within the tracked SPoly (this would be a IntersectionType::NON_BOUND)

	// cycle through each border line in the tracked SPoly:
	auto trackedBorderLinesBegin = trackedSPolyRef->borderLines.begin();
	auto trackedBorderLinesEnd = trackedSPolyRef->borderLines.end();
	for (; trackedBorderLinesBegin != trackedBorderLinesEnd; trackedBorderLinesBegin++)
	{
		TwoDPoint trackedSegmentA = convertGlmVec3To2D(trackedBorderLinesBegin->second.pointA);
		TwoDPoint trackedSegmentB = convertGlmVec3To2D(trackedBorderLinesBegin->second.pointB);
		LineSegment trackedSegment(trackedSegmentA, trackedSegmentB);

		std::cout << "!! Current trackedBorderLine points are: A->" << trackedSegmentA.x << ", " << trackedSegmentA.y << " | " 
														  << " B->" << trackedSegmentB.x << ", " << trackedSegmentB.y << std::endl;

		auto relatedSTrianglesBegin = relatedSPolyRef->triangles.begin();
		auto relatedSTrianglesEnd = relatedSPolyRef->triangles.end();
		for (; relatedSTrianglesBegin != relatedSTrianglesEnd; relatedSTrianglesBegin++)
		{
			for (int x = 0; x < 3; x++)
			{
				TwoDPoint currentSTriangleLineSegmentA = convertGlmVec3To2D(relatedSTrianglesBegin->second.triangleLines[x].pointA);
				TwoDPoint currentSTriangleLineSegmentB = convertGlmVec3To2D(relatedSTrianglesBegin->second.triangleLines[x].pointB);
				LineSegment currentSTriangleLineSegmment(currentSTriangleLineSegmentA, currentSTriangleLineSegmentB);

				std::cout << "--> Comparing trackedBorderLine against currentSTriangleLineSegment with these points: A-> " << currentSTriangleLineSegmentA.x << ", " << currentSTriangleLineSegmentA.y << " | "
																												<< " B-> " << currentSTriangleLineSegmentB.x << ", " << currentSTriangleLineSegmentB.y << std::endl;
				LineSegmentIntersectAnalyzer comparator(trackedSegment, currentSTriangleLineSegmment);


				// perform analysis here.
			}
		}
	}

}

TwoDPoint CoplanarCategorizedLineProducer::convertGlmVec3To2D(glm::vec3 in_glmvec3)
{
	TwoDPoint returnPoint(in_glmvec3.x, in_glmvec3.y);
	return returnPoint;
}