#include "stdafx.h"
#include "CoplanarCategorizedLineProducer.h"

void CoplanarCategorizedLineProducer::performLineComparison()
{
	std::cout << "!!! Performing line comparison..." << std::endl;
	// test the basic line intersection finder class...
	LineSegment lineSegmentA(0, 0, 3, 1);	// a line with point A at 0,0, and point B at 3,1
	LineSegment lineSegmentB(3, 0, 1, 2);	// "" ""
	LineSegmentIntersectAnalyzer analyzer(lineSegmentA, lineSegmentB);
}