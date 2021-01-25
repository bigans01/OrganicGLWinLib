#pragma once

#ifndef SPOLYDOSET_H
#define SPOLYDOSET_H

#include <set>
#include "SPolyDO.h"

class SPolyDOSet
{
	public:
		void insert(SPolyDO in_sPolyDO)
		{
			debugOptions.insert(in_sPolyDO);
		}
		std::set<SPolyDO> debugOptions;
};

#endif
