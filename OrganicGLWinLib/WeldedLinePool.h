#pragma once

#ifndef WELDEDLINEPOOL_H
#define WELDEDLINEPOOL_H

#include "WeldedLine.h"
#include <iostream>
#include <map>

class WeldedLinePool
{
	public:
		void insertLineIntoPool(WeldedLine in_weldedLineToInsert)
		{
			int currentIndex = pool.size();
			pool[currentIndex] = in_weldedLineToInsert;
		}
		void printLines()
		{
			auto poolBegin = pool.begin();
			auto poolEnd = pool.end();
			for (; poolBegin != poolEnd; poolBegin++)
			{
				std::cout << "[" << poolBegin->first << "], Point A: " << poolBegin->second.pointA.x << ", " << poolBegin->second.pointA.y << ", " << poolBegin->second.pointA.z << ", "
					<< "| Point B: " << poolBegin->second.pointB.x << ", " << poolBegin->second.pointB.y << ", " << poolBegin->second.pointB.z << ", "
					<< "| Empty normal: " << poolBegin->second.emptyNormal.x << ", " << poolBegin->second.emptyNormal.y << ", " << poolBegin->second.emptyNormal.z << std::endl;
			}
		}
		int getPoolSize()
		{
			return pool.size();
		}

		WeldedLine fetchLineFromPoolViaIndex(int in_lineIndex)
		{
			return pool[in_lineIndex];
		}
		WeldedLine fetchLastLineInPool()
		{
			auto lastLine = pool.rbegin();
			return lastLine->second;
		}

	private:
		std::map<int, WeldedLine> pool;
};

#endif
