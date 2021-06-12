#include "stdafx.h"
#include "RMorphableAreaScanner.h"

void RMorphableAreaScanner::addSPolyToGrid(SPoly in_sPolyToAdd)
{
	massGrid.addGridRPoly(in_sPolyToAdd);
}

void RMorphableAreaScanner::buildGridMassShell()
{
	massGrid.buildShell();
}

void RMorphableAreaScanner::fillGridMass()
{
	massGrid.fillMass();
}

void RMorphableAreaScanner::scanGridMass()
{

}