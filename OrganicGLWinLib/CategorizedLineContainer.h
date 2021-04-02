#pragma once

#ifndef CATEGORIZEDLINECONTAINER_H
#define CATEGORIZEDLINECONTAINER_H

#include <map>
#include "CategorizedLine.h"

class CategorizedLineContainer
{
	private:
		friend class CategorizedLineContainer;
		friend class CategorizedLineManager;
		int getLineCount()
		{
			return int(lineMap.size());
		}
		int insertLineAndReturnInsertedIndex(CategorizedLine in_categorizedLine)
		{
			int currentIndexToInsertAt = int(lineMap.size());
			lineMap[currentIndexToInsertAt] = in_categorizedLine;
			return currentIndexToInsertAt;
		}
		CategorizedLine fetchAndRemove(int in_indexToFetch)
		{
			CategorizedLine returnLine = lineMap[in_indexToFetch];
			lineMap.erase(in_indexToFetch);
			return returnLine;
		}
		void invertNormals()
		{
			auto lineMapBegin = lineMap.begin();
			auto lineMapEnd = lineMap.end();
			for (; lineMapBegin != lineMapEnd; lineMapBegin++)
			{
				lineMapBegin->second.emptyNormal *= -1.0f;
			}
		}
		void printLines()
		{
			auto lineMapBegin = lineMap.begin();
			auto lineMapEnd = lineMap.end();
			for (; lineMapBegin != lineMapEnd; lineMapBegin++)
			{

			}
		}
	
		std::map<int, CategorizedLine> lineMap;

};

#endif
