#include "stdafx.h"
#include "CoplanarMassCreator.h"

void CoplanarMassCreator::runMassManipulation()
{
	// first, find the total area of the tracked SPoly.
	float totalTrackedArea = calculateSPolyArea(trackedSPolyRef);
}