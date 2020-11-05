#pragma once

#ifndef WELDEDLINEPOOLGUIDE_H
#define WELDEDLINEPOOLGUIDE_H

#include <vector>
#include "WeldedLinePool.h"

class WeldedLinePoolGuide
{
	public:
		WeldedLinePoolGuide() {};
		WeldedLinePoolGuide(int in_currentLineOfSightLineIndex, WeldedLinePool* in_weldedLinePoolRef) : 
			lineOfSightLineIndex(in_currentLineOfSightLineIndex),
			weldedLinePoolRef(in_weldedLinePoolRef)
		{
			buildGuide();
		}

		void buildGuide()
		{
			if (weldedLinePoolRef->getPoolSize() == 4)	// logic for FINAL_OBSERVE
			{
				if (lineOfSightLineIndex == weldedLinePoolRef->getFirstElementID())	// would be at the beginning of the WeldedLinePool's map
				{
					lineOfSightLineIndex = 0;
					observationEndLineIndex = weldedLinePoolRef->getLastElementID();
					rearHook = 1;
					frontHook = 2;
				}
				else
				{
					setBeginIteratorIndex(lineOfSightLineIndex);
					//lineOfSightLineIndex = incrementIndexAndFetchID();

					observationEndLineIndex = fetchPreviousIndex();
					rearHook = incrementIndexAndFetchID();
					frontHook = incrementIndexAndFetchID();
				}
			}
			else  // logic for CONTINUE_OBSERVE
			{
				if (lineOfSightLineIndex == weldedLinePoolRef->getFirstElementID())	// would be at the beginning of the WeldedLinePool's map
				{
					lineOfSightLineIndex = 0;
					observationEndLineIndex = weldedLinePoolRef->getLastElementID();
					rearHook = 1;
					frontHook = 2;
				}
				else
				{
					setBeginIteratorIndex(lineOfSightLineIndex);
					//lineOfSightLineIndex = incrementIndexAndFetchID();

					observationEndLineIndex = fetchPreviousIndex();
					rearHook = incrementIndexAndFetchID();
					frontHook = incrementIndexAndFetchID();
				}
			}
		}

		void printGuideValues()
		{
			std::cout << "####### Printing guide values: " << std::endl;

			std::cout << "| Pool size: " << weldedLinePoolRef->getPoolSize() << std::endl;
			std::cout << "| lineOfSightLineIndex: " << lineOfSightLineIndex << std::endl;
			std::cout << "| rearHook: " << rearHook << std::endl;
			std::cout << "| frontHook: " << frontHook << std::endl;
			std::cout << "| observationEndLineIndex: " << observationEndLineIndex << std::endl;
		}

		int lineOfSightLineIndex;
		int rearHook;
		int frontHook;
		int observationEndLineIndex;

		int currentIndex = 0;
		std::vector<int> comparables;
	private:
		WeldedLinePool* weldedLinePoolRef = nullptr;
		//std::map<int, WeldedLine>::iterator beginIterator;
		void setBeginIteratorIndex(int in_beginIterationIndex)
		{
			//beginIterator = weldedLinePoolRef->getPoolIteratior(in_beginIterationIndex);
			currentIndex = in_beginIterationIndex;
		}
		int incrementIndexAndFetchID()
		{
			int endingMapID = weldedLinePoolRef->getLastElementID();
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

};


#endif
