#pragma once

#ifndef CLEAVESEQUENCEFACTORY_H
#define CLEAVESEQUENCEFACTORY_H

#include <map>
#include "CategorizedLine.h"

class CleaveSequenceFactory
{
	public:
		std::map<int, CategorizedLine> nonboundMap;	// contains NON_BOUND categorized lines.
		std::map<int, CategorizedLine> partialboundMap;	// contains PARTIAL_BOUND categorized lines.
		std::map<int, CategorizedLine> aslicedMap;	// contains A_SLICED categorized lines.
};

#endif
