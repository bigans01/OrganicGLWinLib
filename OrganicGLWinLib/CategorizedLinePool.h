#pragma once

#ifndef CATEGORIZEDLINEPOOL_H
#define CATEGORIZEDLINEPOOL_H

#include "CategorizedLine.h"
#include <vector>
#include <iostream>
#include <map>
#include "SPolyBorderLines.h"
#include "UnsolvedPreciseSolver.h"

class CategorizedLinePool
{
	public:
		std::vector<CategorizedLine> linePool;
		void clearPool()
		{
			linePool.clear();
		}
		void insertLineIntoPool(CategorizedLine in_categorizedLine)
		{
			// first, check whether or not the line already exists.
			int vectorIndex = 0;
			bool wasFound = false;
			auto linePoolBegin = linePool.begin();
			auto linePoolEnd = linePool.end();
			for (; linePoolBegin != linePoolEnd; linePoolBegin++)
			{
				if (checkIfLinePointsMatch(in_categorizedLine, *linePoolBegin) == true)
				{
					wasFound = true;
					break;
				}
				vectorIndex++;
			}

			if (wasFound == false)
			{
				linePool.push_back(in_categorizedLine);
			}
			else if (wasFound == true)
			{
				linePool.erase(linePool.begin() + vectorIndex);
			}
		};
		void copyLinesFromOtherLinePool(CategorizedLinePool* in_categorizedLinePoolRef)
		{
			auto otherLinePoolBegin = in_categorizedLinePoolRef->linePool.begin();
			auto otherLinePoolEnd = in_categorizedLinePoolRef->linePool.end();
			for (; otherLinePoolBegin != otherLinePoolEnd; otherLinePoolBegin++)
			{
				//linePool.push_back(*otherLinePoolBegin);
				insertLineIntoPool(*otherLinePoolBegin);
			}
		};

		void printLinesInPool()
		{
			int x = 0;
			auto linePoolBegin = linePool.begin();
			auto linePoolEnd = linePool.end();
			for (; linePoolBegin != linePoolEnd; linePoolBegin++)
			{
				std::cout << "(";
				if (linePoolBegin->type == IntersectionType::INTERCEPTS_POINT_PRECISE)
				{
					std::cout << "INTERCEPTS_POINT_PRECISE)                   ";
				}
				else if (linePoolBegin->type == IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR)
				{
					std::cout << "INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR) ";
				}
				else if (linePoolBegin->type == IntersectionType::A_SLICE)
				{
					std::cout << "A_SLICE)                                    ";
				}
				else if (linePoolBegin->type == IntersectionType::NON_BOUND)
				{
					std::cout << "NON_BOUND)                                  ";
				}
				else if (linePoolBegin->type == IntersectionType::PARTIAL_BOUND)
				{
					std::cout << "PARTIAL_BOUND)                              ";
				}

				std::cout << " Line " << x << ", point A: " << linePoolBegin->line.pointA.x << ", " << linePoolBegin->line.pointA.y << ", " << linePoolBegin->line.pointA.z
					<< "| point B: " << linePoolBegin->line.pointB.x << ", " << linePoolBegin->line.pointB.y << ", " << linePoolBegin->line.pointB.z
					<< "| empty normal: " << linePoolBegin->emptyNormal.x << ", " << linePoolBegin->emptyNormal.y << ", " << linePoolBegin->emptyNormal.z << std::endl;
			}
		};

		void solveUnsolvedInterceptPointsPrecise(std::map<int, SPolyBorderLines>* in_sPolyBorderLinesMapRef)
		{
			// check for any unsolveds.
			auto unsolvedSearchBegin = linePool.begin();
			auto unsolvedSearchEnd = linePool.end();
			for (; unsolvedSearchBegin != unsolvedSearchEnd; unsolvedSearchBegin++)
			{
				if (unsolvedSearchBegin->type == IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR)
				{
					/*
					glm::vec3 pointToSearchAgainst = unsolvedSearchBegin->line.pointA;	// point A of this line should be what is on the border lines.

					std::cout << "Discovered pointsPrecise to alter; " << std::endl;
					std::cout << "Point to search against is: " << pointToSearchAgainst.x << ", " << pointToSearchAgainst.y << ", " << pointToSearchAgainst.z << std::endl;

					auto borderLinesBegin = (*in_sPolyBorderLinesMapRef).begin();
					auto borderLinesEnd = (*in_sPolyBorderLinesMapRef).end();
					int lastKeyInMap = int((*in_sPolyBorderLinesMapRef).size() - 1);
					int x = 0;
					for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
					{
						if (borderLinesBegin->second.pointB == pointToSearchAgainst)
						{
							std::cout << "!! Point to search found in point B of the border line at index: " << x << std::endl;
							if (x != lastKeyInMap)
							{

							}
							else if (x == lastKeyInMap)
							{

							}
						}
						x++;
					}

					int someVal = 3;
					std::cin >> someVal;
					*/
					glm::vec3 pointToSearchAgainst = unsolvedSearchBegin->line.pointA;
					UnsolvedPreciseSolver solver(*unsolvedSearchBegin, pointToSearchAgainst, in_sPolyBorderLinesMapRef);
					*unsolvedSearchBegin = solver.fetchSolution();
				}
			}
		}
	private:
		bool checkIfLinePointsMatch(CategorizedLine in_lineA, CategorizedLine in_lineB)
		{
			bool matchFound = false;
			if
			(
				(
				(in_lineA.line.pointA == in_lineB.line.pointA)
				&&
				(in_lineA.line.pointB == in_lineB.line.pointB)
				)

				||

				(
				(in_lineA.line.pointA == in_lineB.line.pointB)
				&&
				(in_lineA.line.pointB == in_lineB.line.pointA)
				)
			)
			{
				//std::cout << "!!!! Warning, already existent line found. " << std::endl;
				//int someVal = 3;
				//std::cin >> someVal;
				matchFound = true;
			}
			return matchFound;
		}

};

#endif
