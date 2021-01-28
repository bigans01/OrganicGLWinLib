#include "stdafx.h"
#include "MassZoneBox.h"

void MassZoneBox::insertNewBoundary(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, MassZoneBoxBoundary in_massZoneBoxBoundary, SPolyDOSet in_sPolyDOSet)
{
	boxBoundaries[in_massZoneBoxBoundaryOrientation] = in_massZoneBoxBoundary;
	boxBoundaries[in_massZoneBoxBoundaryOrientation].setBoundarySPolyInPolySet();	// must be called, or program will crash. The boundary SPoly ref should only be set
																					// after the MassZoneBoxBoundary has been copied into the map, since it will correctly point to that newly
																					// allocated memory.
	boxBoundaries[in_massZoneBoxBoundaryOrientation].setDebugOptionsInSPoly(std::move(in_sPolyDOSet));
	std::cout << "!!! Size of box boundaries is now: " << boxBoundaries.size() << std::endl;
};

void MassZoneBox::printBoundaryLineCounts()
{
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << "Counts for boundary NEG_Z: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << "Counts for boundary POS_X: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << "Counts for boundary POS_Z: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << "Counts for boundary NEG_X: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << "Counts for boundary POS_Y: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << "Counts for boundary NEG_Y: ";
		}
		std::cout << boxBoundariesBegin->second.getBoundarySPolyBorderLineCounts() << std::endl;
	}
}

void MassZoneBox::printBoundaries()
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

void MassZoneBox::runSPolyBasedSubZoneAgainstBoundaries(MassSubZone* in_massSubZoneRef)
{
	//std::cout << "!!! border lines of passed in parameter: " << in_massSubZoneRef->sPolyCopy.borderLines.size() << std::endl;
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			//std::cout << "> Comparing against NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			//std::cout << "> Comparing against POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			//std::cout << "> Comparing against POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			//std::cout << "> Comparing against NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			//std::cout << "> Comparing against POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			//std::cout << "> Comparing against NEG_Y: " << std::endl;
		}
		boxBoundariesBegin->second.compareSPolyBasedSubZoneSPolyToBoundarySPolySet(&in_massSubZoneRef->sPolyCopy);
	}
}

void MassZoneBox::printCategorizedLinesInBoundaries()
{
	std::cout << "################################# Printing categorized lines in each boundary: " << std::endl;
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << ">>>>>>> Categorized lines for POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << ">>>>>>> Categorized lines for POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << ">>>>>>> Categorized lines for POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_Y: " << std::endl;
		}
		boxBoundariesBegin->second.printBoundarySPolyCategorizedLines();
	}
}