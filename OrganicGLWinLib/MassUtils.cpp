#include "stdafx.h"
#include "MassUtils.h"

int MassUtils::getMassZoneOrientationIntValue(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation)
{
	int returnValue = 0;
	switch (in_massZoneBoxBoundaryOrientation)
	{
		case MassZoneBoxBoundaryOrientation::NEG_X: { returnValue = 0; break; };
		case MassZoneBoxBoundaryOrientation::NEG_Z: { returnValue = 1; break; };
		case MassZoneBoxBoundaryOrientation::POS_X: { returnValue = 2; break; };
		case MassZoneBoxBoundaryOrientation::POS_Z: { returnValue = 3; break; };
		case MassZoneBoxBoundaryOrientation::POS_Y: { returnValue = 4; break; };
		case MassZoneBoxBoundaryOrientation::NEG_Y: { returnValue = 5; break; };
	}
	return returnValue;
}

MassZoneBoxBoundaryOrientation MassUtils::getMassZoneOrientationEnumValue(int in_massZoneBoundaryID)
{
	MassZoneBoxBoundaryOrientation returnOrientation;
	switch (in_massZoneBoundaryID)
	{
		case 0: { returnOrientation = MassZoneBoxBoundaryOrientation::NEG_X; break; };
		case 1: { returnOrientation = MassZoneBoxBoundaryOrientation::NEG_Z; break; };
		case 2: { returnOrientation = MassZoneBoxBoundaryOrientation::POS_X; break; };
		case 3: { returnOrientation = MassZoneBoxBoundaryOrientation::POS_Z; break; };
		case 4: { returnOrientation = MassZoneBoxBoundaryOrientation::POS_Y; break; };
		case 5: { returnOrientation = MassZoneBoxBoundaryOrientation::NEG_Y; break; };
	}
	return returnOrientation;
}

void MassUtils::printEnumValue(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation)
{
	switch (in_massZoneBoxBoundaryOrientation)
	{
		case MassZoneBoxBoundaryOrientation::NEG_X: { std::cout << " NEG_X "; break; };
		case MassZoneBoxBoundaryOrientation::NEG_Z: { std::cout << " NEG_Z "; break; };
		case MassZoneBoxBoundaryOrientation::POS_X: { std::cout << " POS_X "; break; };
		case MassZoneBoxBoundaryOrientation::POS_Z: { std::cout << " POS_Z "; break; };
		case MassZoneBoxBoundaryOrientation::POS_Y: { std::cout << " POS_Y "; break; };
		case MassZoneBoxBoundaryOrientation::NEG_Y: { std::cout << " NEG_Y "; break; };
	}
}