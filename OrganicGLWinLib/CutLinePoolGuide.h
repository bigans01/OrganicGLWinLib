#pragma once

#ifndef CUTLINEPOOLGUIDE_H
#define CUTLINEPOOLGUIDE_H

#include "CutLinePool.h"

class CutLinePoolGuide
{
	public:
		CutLinePoolGuide() {};
		CutLinePoolGuide(int in_currentLineOfSightLineIndex, CutLinePool* in_weldedLinePoolRef) :
			lineOfSightLineIndex(in_currentLineOfSightLineIndex),
			weldedLinePoolRef(in_weldedLinePoolRef)
		{
			buildGuide(in_currentLineOfSightLineIndex);
		}

		void buildGuide(int in_startingIndex)
		{
			//std::cout << "#######_> Build guide; starting index is: " << in_startingIndex << std::endl;

			if (weldedLinePoolRef->getPoolSize() == 4)	// logic for FINAL_OBSERVE
			{
				//std::cout << "! Branch 1 hit. " << std::endl;

				if (in_startingIndex == weldedLinePoolRef->getFirstElementID())	// would be at the beginning of the WeldedLinePool's map
				{
					setBeginIteratorIndex(in_startingIndex);
					lineOfSightLineIndex = 0;
					observationEndLineIndex = weldedLinePoolRef->getLastElementID();
					rearHook = 1;
					frontHook = 2;

					buildComparableList();
				}
				else
				{
					setBeginIteratorIndex(in_startingIndex);
					//lineOfSightLineIndex = incrementIndexAndFetchID();

					observationEndLineIndex = fetchPreviousIndex();
					rearHook = incrementIndexAndFetchID();
					frontHook = incrementIndexAndFetchID();

					buildComparableList();
				}
			}
			else  // logic for CONTINUE_OBSERVE
			{

				//std::cout << "! Branch 2 hit. " << std::endl;

				if (in_startingIndex == weldedLinePoolRef->getFirstElementID())	// would be at the beginning of the WeldedLinePool's map
				{
					setBeginIteratorIndex(in_startingIndex);
					lineOfSightLineIndex = 0;

					observationEndLineIndex = weldedLinePoolRef->getLastElementID();
					//rearHook = 1;
					//frontHook = 2;
					rearHook = incrementIndexAndFetchID();
					frontHook = incrementIndexAndFetchID();


					//std::cout << ">>> Rear hook is now: " << rearHook << std::endl;
					//std::cout << ">>> Front hook is now: " << frontHook << std::endl;

					buildComparableList();
				}
				else
				{
					//std::cout << "!!! Branch 2.2 hit. " << std::endl;


					setBeginIteratorIndex(in_startingIndex);
					//lineOfSightLineIndex = incrementIndexAndFetchID();

					observationEndLineIndex = fetchPreviousIndex();
					rearHook = incrementIndexAndFetchID();
					frontHook = incrementIndexAndFetchID();


					//std::cout << ">>> Rear hook is now: " << rearHook << std::endl;
					//std::cout << ">>> Front hook is now: " << frontHook << std::endl;

					buildComparableList();
				}
			}

		}

		void printGuideValues()
		{
			//std::cout << "####### Printing guide values: " << std::endl;

			//std::cout << "| Pool size: " << weldedLinePoolRef->getPoolSize() << std::endl;
			//std::cout << "| lineOfSightLineIndex: " << lineOfSightLineIndex << std::endl;
			//std::cout << "| rearHook: " << rearHook << std::endl;
			//std::cout << "| frontHook: " << frontHook << std::endl;
			//std::cout << "| observationEndLineIndex: " << observationEndLineIndex << std::endl;

			auto comparablesBegin = comparables.begin();
			auto comparablesEnd = comparables.end();
			for (; comparablesBegin != comparablesEnd; comparablesBegin++)
			{
				//std::cout << "|| comparable line: " << *comparablesBegin << std::endl;
			}
		}

		void updateGuide(int in_startingIndex, CutLine in_weldedLine)
		{
			weldedLinePoolRef->replaceLinesAtStartingIndexAndUpdate(in_startingIndex, in_weldedLine);
			buildGuide(in_startingIndex);
		};

		int lineOfSightLineIndex;
		int rearHook;
		int frontHook;
		int observationEndLineIndex;

		int currentIndex = 0;
		std::vector<int> comparables;
		CutLinePool* weldedLinePoolRef = nullptr;
	private:
		//std::map<int, WeldedLine>::iterator beginIterator;
		void setBeginIteratorIndex(int in_beginIterationIndex)
		{
			//beginIterator = weldedLinePoolRef->getPoolIteratior(in_beginIterationIndex);
			currentIndex = in_beginIterationIndex;
		}
		int incrementIndexAndFetchID()
		{


			int endingMapID = weldedLinePoolRef->getLastElementID();

			//std::cout << "Increment and index call : currentIndex: " << currentIndex << " || endingID: " << endingMapID << std::endl;

			if (currentIndex != endingMapID)
			{
				currentIndex++;
			}
			else if (currentIndex == endingMapID)
			{
				currentIndex = 0;
			}
			return currentIndex;
		}

		int fetchPreviousIndex()
		{
			int reversableIndex = currentIndex;
			int beginningMapID = weldedLinePoolRef->getFirstElementID();
			if (reversableIndex != beginningMapID)
			{
				reversableIndex--;
			}
			else if (reversableIndex == beginningMapID)
			{
				reversableIndex = weldedLinePoolRef->getLastElementID();
			}
			return reversableIndex;
		}

		void buildComparableList()
		{
			comparables.clear();
			//std::cout << "!!!! building comparable list! " << std::endl;

			int comparableCount = weldedLinePoolRef->getPoolSize() - 4;

			//std::cout << "!!!! comparable count is: " << comparableCount << std::endl;

			for (int x = 0; x < comparableCount; x++)
			{
				int comparableID = incrementIndexAndFetchID();
				comparables.push_back(comparableID);
			}
		}
};

#endif
