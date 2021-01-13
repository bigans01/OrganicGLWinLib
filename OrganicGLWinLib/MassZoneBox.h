#pragma once

#ifndef MASSZONEBOX_H
#define MASSZONEBOX_H

#include <map>
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxBoundary.h"
#include <iostream>
#include "MassSubZone.h"

class MassZoneBox
{
	public:
		std::map<MassZoneBoxBoundaryOrientation, MassZoneBoxBoundary> boxBoundaries;
		void insertNewBoundary(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, MassZoneBoxBoundary in_massZoneBoxBoundary);
		void printBoundaryLineCounts();
		void printBoundaries();
		void printCategorizedLinesInBoundaries();
		void runSPolyBasedSubZoneAgainstBoundaries(MassSubZone* in_massSubZoneRef);
};

#endif
