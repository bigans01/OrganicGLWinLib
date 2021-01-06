#pragma once

#ifndef MASSZONEBOX_H
#define MASSZONEBOX_H

#include <map>
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxBoundary.h"
#include <iostream>

class MassZoneBox
{
	public:
		std::map<MassZoneBoxBoundaryOrientation, MassZoneBoxBoundary> boxBoundaries;
		void insertNewBoundary(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, MassZoneBoxBoundary in_massZoneBoxBoundary)
		{
			boxBoundaries[in_massZoneBoxBoundaryOrientation] = in_massZoneBoxBoundary;
		};
		void printBoundaries()
		{
			auto boxBoundariesBegin = boxBoundaries.begin();
			auto boxBoundariesEnd = boxBoundaries.end();
			for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
			{
				if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
				{
					std::cout << "Points for boundary NEG_Z: " << std::endl;
				}
				else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
				{
					std::cout << "Points for boundary POS_X: " << std::endl;
				}
				else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
				{
					std::cout << "Points for boundary POS_Z: " << std::endl;
				}
				else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
				{
					std::cout << "Points for boundary NEG_X: " << std::endl;
				}
				else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
				{
					std::cout << "Points for boundary POS_Y: " << std::endl;
				}
				else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
				{
					std::cout << "Points for boundary NEG_Y: " << std::endl;
				}

				boxBoundariesBegin->second.printPoints();

			}

			std::cout << "Finished printing MassZoneBoxBoundaries. " << std::endl;
			int finishedPrintVal = 3;
			std::cin >> finishedPrintVal;
		}
};

#endif
