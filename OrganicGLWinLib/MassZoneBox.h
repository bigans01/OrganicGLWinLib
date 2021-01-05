#pragma once

#ifndef MASSZONEBOX_H
#define MASSZONEBOX_H

#include <map>
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxBoundary.h"

class MassZoneBox
{
	public:
		std::map<MassZoneBoxBoundaryOrientation, MassZoneBoxBoundary> boxBoundaries;
		void insertNewBoundary(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, MassZoneBoxBoundary in_massZoneBoxBoundary)
		{
			boxBoundaries[in_massZoneBoxBoundaryOrientation] = in_massZoneBoxBoundary;
		};
};

#endif
