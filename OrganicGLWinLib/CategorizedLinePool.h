#pragma once

#ifndef CATEGORIZEDLINEPOOL_H
#define CATEGORIZEDLINEPOOL_H

#include "CategorizedLine.h"
#include <vector>

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
			linePool.push_back(in_categorizedLine);
		};
		void copyLinesFromOtherLinePool(CategorizedLinePool* in_categorizedLinePoolRef)
		{
			auto otherLinePoolBegin = in_categorizedLinePoolRef->linePool.begin();
			auto otherLinePoolEnd = in_categorizedLinePoolRef->linePool.end();
			for (; otherLinePoolBegin != otherLinePoolEnd; otherLinePoolBegin++)
			{
				linePool.push_back(*otherLinePoolBegin);
			}
		};

};

#endif
