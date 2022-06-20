#include "stdafx.h"
#include "MassUtils.h"

int MassUtils::getMassZoneOrientationIntValue(BoundaryOrientation in_BoundaryOrientation)
{
	int returnValue = 0;
	switch (in_BoundaryOrientation)
	{
		case BoundaryOrientation::NEG_X: { returnValue = 0; break; };
		case BoundaryOrientation::NEG_Z: { returnValue = 1; break; };
		case BoundaryOrientation::POS_X: { returnValue = 2; break; };
		case BoundaryOrientation::POS_Z: { returnValue = 3; break; };
		case BoundaryOrientation::POS_Y: { returnValue = 4; break; };
		case BoundaryOrientation::NEG_Y: { returnValue = 5; break; };
	}
	return returnValue;
}

BoundaryOrientation MassUtils::getMassZoneOrientationEnumValue(int in_massZoneBoundaryID)
{
	BoundaryOrientation returnOrientation;
	switch (in_massZoneBoundaryID)
	{
		case 0: { returnOrientation = BoundaryOrientation::NEG_X; break; };
		case 1: { returnOrientation = BoundaryOrientation::NEG_Z; break; };
		case 2: { returnOrientation = BoundaryOrientation::POS_X; break; };
		case 3: { returnOrientation = BoundaryOrientation::POS_Z; break; };
		case 4: { returnOrientation = BoundaryOrientation::POS_Y; break; };
		case 5: { returnOrientation = BoundaryOrientation::NEG_Y; break; };
	}
	return returnOrientation;
}

void MassUtils::printEnumValue(BoundaryOrientation in_BoundaryOrientation)
{
	switch (in_BoundaryOrientation)
	{
		case BoundaryOrientation::NEG_X: { std::cout << " NEG_X "; break; };
		case BoundaryOrientation::NEG_Z: { std::cout << " NEG_Z "; break; };
		case BoundaryOrientation::POS_X: { std::cout << " POS_X "; break; };
		case BoundaryOrientation::POS_Z: { std::cout << " POS_Z "; break; };
		case BoundaryOrientation::POS_Y: { std::cout << " POS_Y "; break; };
		case BoundaryOrientation::NEG_Y: { std::cout << " NEG_Y "; break; };
	}
}