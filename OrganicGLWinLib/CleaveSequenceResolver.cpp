#include "stdafx.h"
#include "CleaveSequenceResolver.h"

void CleaveSequenceResolver::addRPolyToResolve(SPoly in_sPolyToResolve)
{
	RPoly polyToResolve(in_sPolyToResolve, numberOfTilesPerDimension, dimensionLimit, rCubeDimensionalLength, tileDimWeightToHundredthFloatRatio);
	rPolyToResolve = polyToResolve;
}

void CleaveSequenceResolver::addRPolyToCompare(SPoly in_comparingSPoly)
{

}

/*
unsigned short CleaveSequenceResolver::convertBlockCoordsToUShort(int in_x, int in_y, int in_z)
{
	int x = in_x * (numberOfTilesPerDimension*numberOfTilesPerDimension);
	int y = in_y * (numberOfTilesPerDimension);
	return x + y + in_z;
}

EnclaveKeyDef::EnclaveKey CleaveSequenceResolver::convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert)
{
	return RUtils::convertToRasterGridCell(in_pointToConvert, rCubeDimensionalLength, numberOfTilesPerDimension);
}
*/

void CleaveSequenceResolver::printRPolyToResolvePoints()
{
	rPolyToResolve.printRPoints();
}