#include "stdafx.h"
#include "SPolyShellValidityChecker.h"

void SPolyShellValidityChecker::insertLine(HashedSTriangleLine in_lineToInsert)
{
	if (counterTracker.empty())
	{
		HashedSTriangleLineCounter newCounter(in_lineToInsert);
		counterTracker[0] = newCounter;
	}
	else
	{
		bool lineFound = false;

		// If didLinesMatch below is true,  we found the line -- incremented the counter -- and we're done.
		for (auto& currentTracker : counterTracker)
		{
			bool didLinesMatch = currentTracker.second.checkIfLinesMatch(&in_lineToInsert);
			if (didLinesMatch)
			{
				lineFound = true;
				break;
			}
		}

		// ...otherwise, insert a new line.
		if (!lineFound)
		{
			HashedSTriangleLineCounter newCounter(in_lineToInsert);
			counterTracker[counterTracker.size()] = newCounter;
		}
	}
}

bool SPolyShellValidityChecker::fetchValidity()
{
	return calculateValidity();
}

void SPolyShellValidityChecker::printLineStats()
{
	std::cout << "!!! Printing out hash lines..." << std::endl;
	for (auto& currentTracker : counterTracker)
	{
		std::cout << "[" << currentTracker.first << "] ";
		currentTracker.second.printCounterStats();
	}
}

bool SPolyShellValidityChecker::calculateValidity()
{
	bool isValid = true;	// as soon as a tracker doesn't have 2 in it, we know it's bad.
	for (auto& currentTrackerToCheck : counterTracker)
	{
		if (currentTrackerToCheck.second.getMatchCount() != 2)
		{
			isValid = false;
		}
	}

	return isValid;
}