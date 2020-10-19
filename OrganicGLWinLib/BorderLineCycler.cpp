#include "stdafx.h"
#include "BorderLineCycler.h"

void BorderLineCycler::findCyclingDirection(SPolyBorderLines* in_borderLineRef, int in_borderLineIndex, CategorizedLine* in_categorizedLineRef, MassManipulationMode in_massManipulationMode)
{
	//std::cout << "#- Cycling Direction    > Finding cycling direction for \"Z-Planar\" border line, by checking the first line in the first CleaveSequence..." << std::endl;
	//std::cout << "#- Cycling Direction    > Referenced border line points, prior to empty normal re-orientation: point A " << in_borderLineRef->pointA.x << ", " << in_borderLineRef->pointA.y << ", " << in_borderLineRef->pointA.z
		//      << " | point B: " << in_borderLineRef->pointB.x << ", " << in_borderLineRef->pointB.y << ", " << in_borderLineRef->pointB.z << std::endl;

	//std::cout << "#- Cycling Direction    > Referenced categorized line points, prior to empty normal re-orientation: point A " << in_categorizedLineRef->line.pointA.x << ", " << in_categorizedLineRef->line.pointA.y << ", " << in_categorizedLineRef->line.pointA.z
		//      << " | point B " << in_categorizedLineRef->line.pointB.x << ", " << in_categorizedLineRef->line.pointB.y << ", " << in_categorizedLineRef->line.pointB.z << std::endl;
	//std::cout << "#- Cycling Direction    > Empty normal value, prior to its re-orientation: " << in_categorizedLineRef->emptyNormal.x << ", " << in_categorizedLineRef->emptyNormal.y << ", " << in_categorizedLineRef->emptyNormal.z << std::endl;

	// find the cycling direction,
	//auto truestart = std::chrono::high_resolution_clock::now();
	CyclingDirectionFinder cycleFinder(*in_borderLineRef, *in_categorizedLineRef, in_massManipulationMode);

	//auto trueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> trueelapsed2 = trueend - truestart;
	//std::cout << "#-> (CyclingDirectionFinder) constructor Time !!  > " << std::fixed << trueelapsed2.count() << std::endl;

	direction = cycleFinder.foundDirection;	// store the direction
}

void BorderLineCycler::buildCycle(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex)
{
	//auto truestart = std::chrono::high_resolution_clock::now();
	lineCycle.initialize(in_sPolyRef, in_borderLineStartIndex, in_borderLineEndIndex, direction);
	//auto trueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> trueelapsed2 = trueend - truestart;
	//std::cout << "#-> (BuildLineCycler) initialize function Time !!  > " << std::fixed << trueelapsed2.count() << std::endl;
}