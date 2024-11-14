#include "stdafx.h"
#include "HashedSTriangleLineCounter.h"

HashedSTriangleLineCounter::HashedSTriangleLineCounter()
{

}

HashedSTriangleLineCounter::HashedSTriangleLineCounter(HashedSTriangleLine in_line)
{
	lineToCount = in_line;
}

bool HashedSTriangleLineCounter::checkIfLinesMatch(HashedSTriangleLine* in_line)
{
	bool linesMatch = false;

	if (lineToCount.doLinesMatch(in_line))
	{
		//std::cout << "....incrementing counter. " << std::endl;
		//std::cout << "pre increment, value is now: " << matchCount << std::endl;
		matchCount++;
		//std::cout << "post increment, value is now: " << matchCount << std::endl;
		linesMatch = true;
	}

	return linesMatch;
}

int HashedSTriangleLineCounter::getMatchCount()
{
	return matchCount;
}

void HashedSTriangleLineCounter::printCounterStats()
{
	std::cout << "! Hashed line stats -> matchCount: " << matchCount << " ";
	lineToCount.printHashedLineStats();
}