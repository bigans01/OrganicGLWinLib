#pragma once

#ifndef CUTLINEPOOL_H
#define CUTLINEPOOL_H

#include <map>
#include "CutLine.h"

class CutLinePool
{
	public:
		std::map<int, CutLine> cutLineMap;
};

#endif
