#pragma once

#ifndef CUTLINEPOOL_H
#define CUTLINEPOOL_H

#include <map>
#include "CutLine.h"

class CutLinePool
{
	public:
		void insertLineIntoPool(CutLine in_weldedLineToInsert)
		{
			int currentIndex = int(pool.size());
			pool[currentIndex] = in_weldedLineToInsert;
		}
	private:
		std::map<int, CutLine> pool;
};

#endif
