#pragma once

#ifndef CONTESTABLEBORDERLINEMAP_H
#define CONTESTABLEBORDERLINEMAP_H

#include <vector>
#include <map>
#include "CategorizedLine.h"

class ContestableBorderLineMap
{
	public:
		void insertChallengingCategorizedLine(int in_borderLineID, CategorizedLine in_categorizedLine);
	private:
		friend class MassZoneBoxBoundarySPolySet;
		std::map<int, std::vector<CategorizedLine>> contestableBorderLineChallengersMap;
};

#endif
