#include "stdafx.h"
#include "CoplanarCategorizedLineProducer.h"

void CoplanarCategorizedLineProducer::performLineComparison()
{
	std::cout << "!!! Performing line comparison..." << std::endl;
	// test the basic line intersection finder class...

	// typical intersect test.
	//LineSegment lineSegmentA(0, 0, 3, 1);	// a line with point A at 0,0, and point B at 3,1	-- should result in a t of .75
	//LineSegment lineSegmentA(3, 1, 0, 0);	// a line with point A at 3,1, and point B at 0,0	-- should result in a t of .25
	//LineSegment lineSegmentB(3, 0, 1, 2);	// "" ""


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
	LineSegment lineSegmentA(0, 0, 2, 0);
	LineSegment lineSegmentB(3, 5, 5, 0);




	LineSegmentIntersectAnalyzer analyzer(lineSegmentA, lineSegmentB);
}