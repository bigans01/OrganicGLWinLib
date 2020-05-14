#include "stdafx.h"
#include "BorderLineCycle.h"

void BorderLineCycle::initialize(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex, CyclingDirection in_direction)
{
	sPolyRef = in_sPolyRef;
	startIndex = in_borderLineStartIndex;
	endIndex = in_borderLineEndIndex;
	direction = in_direction;
	numberOfBorderLines = sPolyRef->numberOfBorderLines;
	borderLineFinalIndex = numberOfBorderLines - 1;
	calculateNumberOfIterations();

	std::cout << "!! Border line cycle start index: " << startIndex << std::endl;
	std::cout << "!! Border line cycle end index: " << endIndex << std::endl;
}

void BorderLineCycle::calculateNumberOfIterations()
{
	if (direction == CyclingDirection::FORWARD)
	{
		firstIterationSetNumberOfIterations = (borderLineFinalIndex - startIndex) + 1;
		secondIterationSetNumberOfIterations = startIndex;

		std::cout << "FORWARD iteration values: first set: " << firstIterationSetNumberOfIterations << " | second set: " << secondIterationSetNumberOfIterations << std::endl;
	}
	else if (direction == CyclingDirection::REVERSE)
	{
		firstIterationSetNumberOfIterations = startIndex + 1;
		secondIterationSetNumberOfIterations = (borderLineFinalIndex - startIndex);

		std::cout << "REVERSE iteration values: first set: " << firstIterationSetNumberOfIterations << " | second set: " << secondIterationSetNumberOfIterations << std::endl;
	}
}