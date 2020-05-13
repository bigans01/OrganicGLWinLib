#include "stdafx.h"
#include "BorderLineCycler.h"

void BorderLineCycler::findCyclingDirection(SPolyBorderLines* in_borderLineRef, int in_borderLineIndex, CategorizedLine* in_categorizedLineRef)
{
	std::cout << "!! (Border Line) : point A " << in_borderLineRef->pointA.x << ", " << in_borderLineRef->pointA.y << ", " << in_borderLineRef->pointA.z
		<< " | point B: " << in_borderLineRef->pointB.x << ", " << in_borderLineRef->pointB.y << ", " << in_borderLineRef->pointB.z << std::endl;

	std::cout << "!! (Categorized Line) : point A " << in_categorizedLineRef->line.pointA.x << ", " << in_categorizedLineRef->line.pointA.y << ", " << in_categorizedLineRef->line.pointA.z
		<< " | point B " << in_categorizedLineRef->line.pointB.x << ", " << in_categorizedLineRef->line.pointB.y << ", " << in_categorizedLineRef->line.pointB.z << std::endl
		<< " empty normal: " << in_categorizedLineRef->emptyNormal.x << ", " << in_categorizedLineRef->emptyNormal.y << ", " << in_categorizedLineRef->emptyNormal.z << std::endl;

	// find the cycling direction,
	CyclingDirectionFinder(*in_borderLineRef, *in_categorizedLineRef);
				
}