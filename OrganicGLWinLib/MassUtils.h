#pragma once

#ifndef MASSUTILS_H
#define MASSUTILS_H

#include "MassZoneBoxBoundaryOrientation.h"
#include <iostream>

class MassUtils
{
	public:
		static int getMassZoneOrientationIntValue(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation);
		static MassZoneBoxBoundaryOrientation getMassZoneOrientationEnumValue(int in_massZoneBoundaryID);
		static void printEnumValue(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation);
};

#endif
