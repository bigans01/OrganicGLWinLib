#pragma once

#ifndef WELDEDLINEPOOL_H
#define WELDEDLINEPOOL_H

#include "WeldedLine.h"

class WeldedLinePool
{
	public:
		void insertLineIntoPool(WeldedLine in_weldedLineToInsert)
		{
			int currentIndex = int(pool.size());
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

		std::vector<std::string> printLinesToStringVector()
		{
			std::vector<std::string> lineStringVector;
			for (auto& currentLine : pool)
			{
				std::string currentLineString = "Line ID: " + std::to_string(currentLine.first) + " | Point A -> ("
					+ std::to_string(currentLine.second.pointA.x) + ", " + std::to_string(currentLine.second.pointA.y) + ", " + std::to_string(currentLine.second.pointA.z)
					+ ") | Point B -> (" + std::to_string(currentLine.second.pointB.x) + ", " + std::to_string(currentLine.second.pointB.y) + ", " + std::to_string(currentLine.second.pointB.z) + ") | Normal -> "
					+ std::to_string(currentLine.second.emptyNormal.x) + ", " + std::to_string(currentLine.second.emptyNormal.y) + ", " + std::to_string(currentLine.second.emptyNormal.z) + ") ";
				lineStringVector.push_back(currentLineString);
			}
			return lineStringVector;
		}

		int getPoolSize()
		{
			return int(pool.size());
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
				int numberOfRemainingLinesToIterate = int(pool.size() - 3);
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
				std::cout << "::::::::::::: END::::: END TEST |" << std::endl;
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
				int numberOfRemainingLinesToIterate = int(pool.size() - 3);
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

		void purgeNegligibleLines()	
		{
			// number of purge attempts is equivalent to the number of short lines discovered
			auto purgeScanPassBegin = pool.begin();
			auto purgeScanPassEnd = pool.end();
			int purgeAttemptCounter = 0;
			for (; purgeScanPassBegin != purgeScanPassEnd; purgeScanPassBegin++)
			{
				glm::vec3 pointA = purgeScanPassBegin->second.pointA;
				glm::vec3 pointB = purgeScanPassBegin->second.pointB;
				float lineDist = glm::distance(pointA, pointB);
				if (lineDist <= .01f)
				{
					purgeAttemptCounter++;
				}
			}

			for (int x = 0; x < purgeAttemptCounter; x++)
			{
				runSingleLinePurgeAttempt();
			}
		}

		void runSingleLinePurgeAttempt()
		{
			if (pool.size() > 3)
			{
				int currentLineIndex = 0;
				int nextLineIndex = 0;
				int endLineIndex = pool.rbegin()->first;

				auto poolBegin = pool.begin();
				auto poolEnd = pool.end();
				bool wasShortLineFound = false;
				int targetLineToRemove = 0;
				for (; poolBegin != poolEnd; poolBegin++)
				{
					glm::vec3 pointA = poolBegin->second.pointA;
					glm::vec3 pointB = poolBegin->second.pointB;
					float lineDist = glm::distance(pointA, pointB);
					if (lineDist <= .01f)
					{
						/*
						std::cout << "!!! Notice, lines too close...continue? " << std::endl;
						int waitVal = 3;
						while (waitVal == 3)
						{

						}
						*/
						//std::cout << "++++++++++++++++++++++++ Prior to short line erase at index " << poolBegin->first << "  lines are: " << std::endl;
						//printLines();


						// take point A of the short line, and copy it into the next line's point A.
						glm::vec3 pointCopy = pointA;
						if (poolBegin->first == endLineIndex)	// the short line is the last line in the map; so replace point A at the beginning of the map.
						{
							auto poolStart = pool.begin();
							poolStart->second.pointA = pointCopy;
						}
						else  // otherwise, the short line isn't at the end, so the point to replace is in the next line.
						{
							auto nextLineIter = poolBegin;
							nextLineIter++;
							nextLineIter->second.pointA = pointCopy;
						}

						targetLineToRemove = poolBegin->first;
						wasShortLineFound = true;
						break;
					}
				}

				if (wasShortLineFound == true)
				{
					// erase the line
					pool.erase(targetLineToRemove);

					// rebuild the pool
					std::map<int, WeldedLine> newPool;
					auto oldPoolBegin = pool.begin();
					auto oldPoolEnd = pool.end();
					for (; oldPoolBegin != oldPoolEnd; oldPoolBegin++)
					{
						int currentNewPoolIndex = int(newPool.size());
						newPool[currentNewPoolIndex] = oldPoolBegin->second;
					}

					// copy reformed pool back
					pool = newPool;

					//std::cout << "++++++++++++++ Erased a short line, remaining pool lines are: " << std::endl;
					//printLines();
					//int waitVal = 3;
					//std::cin >> waitVal;
				}
			}
		}

		void clearPool()
		{
			pool.clear();
		}

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
