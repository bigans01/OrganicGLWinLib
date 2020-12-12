#pragma once

#ifndef CATEGORIZEDLINEPOOL_H
#define CATEGORIZEDLINEPOOL_H

#include "CategorizedLine.h"
#include <vector>
#include <iostream>

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
				std::cout << "Line " << x << ", point A: " << linePoolBegin->line.pointA.x << ", " << linePoolBegin->line.pointA.y << ", " << linePoolBegin->line.pointA.z
					<< "| point B: " << linePoolBegin->line.pointB.x << ", " << linePoolBegin->line.pointB.y << ", " << linePoolBegin->line.pointB.z
					<< "| empty normal: " << linePoolBegin->emptyNormal.x << ", " << linePoolBegin->emptyNormal.y << ", " << linePoolBegin->emptyNormal.z << std::endl;
			}
		};
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
