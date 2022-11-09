#pragma once

#ifndef MASSUTILS_H
#define MASSUTILS_H


class MassUtils
{
	public:
		static int getMassZoneOrientationIntValue(BoundaryOrientation in_BoundaryOrientation);
		static BoundaryOrientation getMassZoneOrientationEnumValue(int in_massZoneBoundaryID);
		static void printEnumValue(BoundaryOrientation in_BoundaryOrientation);
};

#endif
