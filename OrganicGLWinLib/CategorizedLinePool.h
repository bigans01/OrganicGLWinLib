#pragma once

#ifndef CATEGORIZEDLINEPOOL_H
#define CATEGORIZEDLINEPOOL_H

#include "CategorizedLine.h"
#include <vector>

class CategorizedLinePool
{
	public:
		std::vector<CategorizedLine> linePool;
		void insertLineIntoPool(CategorizedLine in_categorizedLine)
		{
			linePool.push_back(in_categorizedLine);
		};
};

#endif
