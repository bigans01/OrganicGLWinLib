#include "stdafx.h"
#include "BorderLineCycle.h"

void BorderLineCycle::initialize(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex, CyclingDirection in_direction)
{
	sPolyRef = in_sPolyRef;
	startIndex = in_borderLineStartIndex;
	endIndex = in_borderLineEndIndex;
	direction = in_direction;
	numberOfBorderLines = sPolyRef->numberOfSPolyBorderLines;
	borderLineFinalIndex = numberOfBorderLines - 1;
	calculateNumberOfIterations();

	//std::cout << "!! Border line cycle start index: " << startIndex << std::endl;
	//std::cout << "!! Border line cycle end index: " << endIndex << std::endl;
}

void BorderLineCycle::calculateNumberOfIterations()
{
	if (direction == CyclingDirection::FORWARD)
	{
		firstIterationSetNumberOfIterations = (borderLineFinalIndex - startIndex) + 1;
		secondIterationSetNumberOfIterations = startIndex;

		//std::cout << "FORWARD iteration values: first set: " << firstIterationSetNumberOfIterations << " | second set: " << secondIterationSetNumberOfIterations << std::endl;
	}
	else if (direction == CyclingDirection::REVERSE)
	{
		firstIterationSetNumberOfIterations = startIndex + 1;
		secondIterationSetNumberOfIterations = (borderLineFinalIndex - startIndex);

		//std::cout << "REVERSE iteration values: first set: " << firstIterationSetNumberOfIterations << " | second set: " << secondIterationSetNumberOfIterations << std::endl;
	}
	int totalRequiredIterations = getTotalIterationCount();
	populateCycleArray();		// populate the cycling array.
}

int BorderLineCycle::getTotalIterationCount()
{
	int totalIterations = 0;
	bool stopFlag = false;		// indicates when to stop.
	int currentIndex = 0;		// the current index we are on.
	if (direction == CyclingDirection::FORWARD)
	{
		// go through the first set, forward
		currentIndex = startIndex;
		for (int x = 0; x < firstIterationSetNumberOfIterations; x++)
		{
			if (currentIndex == endIndex) // we must stop
			{
				totalIterations++;
				stopFlag = true;
				break;
			}
			else
			{
				totalIterations++;		// only iterate until we stop.
			}
			currentIndex++;
		}

		// go through the second set, only if the stop flag is still false.
		if (stopFlag == false)
		{
			currentIndex = 0;	// always start at index 0 in the second set, when going forward.
			for (int x = 0; x < secondIterationSetNumberOfIterations; x++)
			{
				if (currentIndex == endIndex) // we must stop
				{
					totalIterations++;
					stopFlag = true;
					break;
				}
				else
				{
					totalIterations++;		// only iterate until we stop.
				}
				currentIndex++;
			}
		}

	}
	else if (direction == CyclingDirection::REVERSE)
	{
		// go through the first set, reverse
		currentIndex = startIndex;
		for (int x = firstIterationSetNumberOfIterations; x > 0; x--)
		{
			if (currentIndex == endIndex)
			{
				totalIterations++;
				stopFlag = true;
				break;
			}
			else
			{
				totalIterations++;
			}
			currentIndex--;
		}

		// go through the second set, only if the stop flag is still false.
		if (stopFlag == false)
		{
			currentIndex = borderLineFinalIndex;	// when going reverse, the second set's first index value is the last index
			for (int x = secondIterationSetNumberOfIterations; x > 0; x--)
			{
				if (currentIndex == endIndex) // we must stop
				{
					totalIterations++;
					stopFlag = true;
					break;
				}
				else
				{
					totalIterations++;		// only iterate until we stop.
				}
				currentIndex--;
			}
		}
	}

	// std::cout << "Total iterations: " << totalIterations << std::endl;
	return totalIterations;
}

void BorderLineCycle::populateCycleArray()
{
	int currentIndex = 0;		// the current index we are on.
	if (direction == CyclingDirection::FORWARD)
	{
		// go through the first set, forward; current index is always the start index that was established earlier.
		int arrayIndex = 0;
		currentIndex = startIndex;
		for (int x = 0; x < firstIterationSetNumberOfIterations; x++)
		{
			std::cout << "Array element at " << arrayIndex++ << " will be for " << currentIndex++ << std::endl;
		}

		currentIndex = 0;
		for (int x = 0; x < secondIterationSetNumberOfIterations; x++)
		{
			std::cout << "Array element at " << arrayIndex++ << " will be for " << currentIndex++ << std::endl;
		}
	}
	else if (direction == CyclingDirection::REVERSE)
	{
		int arrayIndex = 0;
		currentIndex = startIndex;
		for (int x = firstIterationSetNumberOfIterations; x > 0; x--)
		{
			std::cout << "Array element at " << arrayIndex++ << " will be for " << currentIndex-- << std::endl;
		}

		currentIndex = borderLineFinalIndex;
		for (int x = secondIterationSetNumberOfIterations; x > 0; x--)
		{
			std::cout << "Array element at " << arrayIndex++ << " will be for " << currentIndex-- << std::endl;
		}
	}
}