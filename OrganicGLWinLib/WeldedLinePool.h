#pragma once

#ifndef WELDEDLINEPOOL_H
#define WELDEDLINEPOOL_H

#include "WeldedLine.h"
#include  <map>

class WeldedLinePool
{
	public:
		void insertLineIntoPool(WeldedLine in_weldedLineToInsert)
		{
			int currentIndex = pool.size();
			pool[currentIndex] = in_weldedLineToInsert;
		}
	private:
		std::map<int, WeldedLine> pool;
};

#endif
