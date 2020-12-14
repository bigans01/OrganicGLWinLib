#include "stdafx.h"
#include "UnsolvedPreciseSolver.h"

void UnsolvedPreciseSolver::generateSolution()
{
	// cycle through the border lines
	auto borderLinesBegin = (*sPolyBorderLinesMapRef).begin();
	auto borderLinesEnd = (*sPolyBorderLinesMapRef).end();
	int lastKeyInMap = int((*sPolyBorderLinesMapRef).size() - 1);
	int x = 0;
	int firstBorderLine = 0;
	int secondBorderLine = 0;
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		if (borderLinesBegin->second.pointB == pointToSolveFor)
		{
			std::cout << "!! Point to search found in point B of the border line at index: " << x << std::endl;
			if (x != lastKeyInMap)
			{
				firstBorderLine = x;
				secondBorderLine = x + 1;
			}
			else if (x == lastKeyInMap)
			{
				firstBorderLine = x;
				secondBorderLine = 0;
			}
		}
		x++;
	}

	std::cout << ":: UnsolvedPreciseSolver: firstBorderLine: " << firstBorderLine << std::endl;
	std::cout << ":: UnsolvedPreciseSolver: secondBorderLine: " << secondBorderLine << std::endl;

	// generate data for the new line.
	producedLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
	producedLine.line.pointA = lineToSolveFor.line.pointA;
	producedLine.line.pointB = lineToSolveFor.line.pointB;
	producedLine.line.numberOfBorderLines = 1;
	producedLine.line.pointABorder = firstBorderLine;
	producedLine.line.pointBBorder = secondBorderLine;
	producedLine.emptyNormal = lineToSolveFor.emptyNormal;

	std::cout << ":: produced line stats: " << std::endl;
	std::cout << "point A: " << producedLine.line.pointA.x << ", " << producedLine.line.pointA.y << ", " << producedLine.line.pointA.z << std::endl;
	std::cout << "point B: " << producedLine.line.pointB.x << ", " << producedLine.line.pointB.y << ", " << producedLine.line.pointB.z << std::endl;
	std::cout << "border 1: " << producedLine.line.pointABorder << std::endl;
	std::cout << "border 2: " << producedLine.line.pointBBorder << std::endl;
	std::cout << "empty normal: " << producedLine.emptyNormal.x << ", " << producedLine.emptyNormal.y << ", " << producedLine.emptyNormal.z << std::endl;


	std::cout << "Line produced. Continue? " << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	
}

CategorizedLine UnsolvedPreciseSolver::fetchSolution()
{
	return producedLine;
}