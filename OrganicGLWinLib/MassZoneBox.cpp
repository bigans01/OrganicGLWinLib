#include "stdafx.h"
#include "MassZoneBox.h"

void MassZoneBox::runSPolyBasedSubZoneAgainstBoundaries(MassSubZone* in_massSubZoneRef)
{
	std::cout << "!!! border lines of passed in parameter: " << in_massSubZoneRef->sPolyCopy.borderLines.size() << std::endl;
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << "> Comparing against NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << "> Comparing against POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << "> Comparing against POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << "> Comparing against NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << "> Comparing against POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << "> Comparing against NEG_Y: " << std::endl;
		}
		boxBoundariesBegin->second.compareSPolyBasedSubZoneSPolyToBoundarySPolySet(&in_massSubZoneRef->sPolyCopy);
	}
}