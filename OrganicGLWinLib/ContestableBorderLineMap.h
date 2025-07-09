#pragma once

#ifndef CONTESTABLEBORDERLINEMAP_H
#define CONTESTABLEBORDERLINEMAP_H

#include "CategorizedLine.h"

/* Description: this class is used to store CategorizedLine objects that have been flagged as being 
colinear to a border line of an STriangle; used primarily in MassZoneBoxBoundarySPolySet::insertCategorizedLinesFromNonboundarySPoly. */

class ContestableBorderLineMap
{
	public:
		void insertChallengingCategorizedLine(int in_borderLineID, CategorizedLine in_categorizedLine);
	private:
		friend class MassZoneBoxBoundarySPolySet;
		std::map<int, std::vector<CategorizedLine>> contestableBorderLineChallengersMap;
};

#endif
