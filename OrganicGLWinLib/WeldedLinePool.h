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

		int getFirstElementID()
		{
			auto firstLine = pool.begin();
			return firstLine->first;
		}

		int getLastElementID()
		{
			auto lastLine = pool.rbegin();
			return lastLine->first;
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
		std::map<int, WeldedLine>::iterator getPoolIteratior(int in_lineIndex)
		{
			return pool.find(in_lineIndex);
		}
		int fetchFollowingIndex(int in_lineIndex)
		{
			int returnValue = 0;
			auto lastLineID = pool.rbegin()->first;
			if (in_lineIndex == lastLineID)
			{
				returnValue = 0;
			}
			else
			{
				returnValue = in_lineIndex + 1;
			}
			return returnValue;
		}

		void replaceLinesAtStartingIndexAndUpdate(int in_startingIndex, WeldedLine in_line)
		{
			std::map<int, WeldedLine> updatedPool;


			// are we at the beginning?
			if (in_startingIndex == 0)
			{
				int numberOfRemainingLinesToIterate = pool.size() - 3;
				auto linesBegin = pool.begin();
				incrementIterator(&linesBegin);	//linesBegin++;
				incrementIterator(&linesBegin);	//linesBegin++;

				updatedPool[0] = in_line;	// the very first line will be the updated line
				int newLinesStartingIndex = findStartingIndexForNewLines(in_startingIndex);
				updatedPool[newLinesStartingIndex] = linesBegin->second;

				// first two lines in new pool are:

				/*
				auto poolBegin = updatedPool.begin();
				auto poolEnd = updatedPool.end();

				std::cout << "::::::::::::: TEST: first lines in updatedPool..." << std::endl;
				for (; poolBegin != poolEnd; poolBegin++)
				{
					std::cout << "[" << poolBegin->first << "], Point A: " << poolBegin->second.pointA.x << ", " << poolBegin->second.pointA.y << ", " << poolBegin->second.pointA.z << ", "
						<< "| Point B: " << poolBegin->second.pointB.x << ", " << poolBegin->second.pointB.y << ", " << poolBegin->second.pointB.z << ", "
						<< "| Empty normal: " << poolBegin->second.emptyNormal.x << ", " << poolBegin->second.emptyNormal.y << ", " << poolBegin->second.emptyNormal.z << std::endl;
				}
				std::cout << "::::::::::::: END TEST |" << std::endl;
				*/


				for (int x = 0; x < numberOfRemainingLinesToIterate; x++)
				{
					incrementIterator(&linesBegin);	//linesBegin++;
					newLinesStartingIndex = fetchNextStartingLineIndex(newLinesStartingIndex);
					updatedPool[newLinesStartingIndex] = linesBegin->second;
				}
			}

			// we're in the middle, but not at the end.
			else if
			(
				(in_startingIndex != 0)
				&&
				(in_startingIndex != pool.rbegin()->first)
			)
			{
				int numberOfRemainingLinesToIterate = pool.size() - 3;
				auto linesBegin = pool.find(in_startingIndex);
				incrementIterator(&linesBegin);	//linesBegin++;
				incrementIterator(&linesBegin);	//linesBegin++;

				updatedPool[in_startingIndex] = in_line;
				int newLinesStartingIndex = findStartingIndexForNewLines(in_startingIndex);
				updatedPool[newLinesStartingIndex] = linesBegin->second;

				for (int x = 0; x < numberOfRemainingLinesToIterate; x++)
				{
					//updatedPool[fetchNextStartingLineIndex(newLinesStartingIndex)] = linesBegin->second;
					//incrementIterator(&linesBegin);
					incrementIterator(&linesBegin);	//linesBegin++;
					newLinesStartingIndex = fetchNextStartingLineIndex(newLinesStartingIndex);
					updatedPool[newLinesStartingIndex] = linesBegin->second;
				}
			}

			pool = updatedPool;

			//std::cout << ":::::::: Values of pool, post update: " << std::endl;
			//printLines();
		};

	private:
		std::map<int, WeldedLine> pool;
		void incrementIterator(std::map<int, WeldedLine>::iterator* in_iterator)
		{
			auto lastElementKey = pool.rbegin()->first;
			if ((*in_iterator)->first == lastElementKey)
			{
				(*in_iterator) = pool.begin();
			}
			else
			{
				(*in_iterator)++;
			}
		}
		int findStartingIndexForNewLines(int in_originalStartIndex)
		{
			int returnIndex = 0;
			auto lastElementKey = pool.rbegin()->first;
			if (in_originalStartIndex == lastElementKey)
			{
				returnIndex = 0;
			}
			else
			{
				returnIndex = in_originalStartIndex + 1;
			}
			return returnIndex;
		}
		int fetchNextStartingLineIndex(int in_currentIndexValue)
		{
			int returnIndex = in_currentIndexValue;
			int lastElementValueProxy = in_currentIndexValue + 1;
			if (lastElementValueProxy == pool.rbegin()->first)
			{
				returnIndex = 0;
			}
			else
			{
				returnIndex++;
			}
			return returnIndex;
		}
};

#endif
