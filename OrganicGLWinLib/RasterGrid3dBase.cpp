#include "stdafx.h"
#include "RasterGrid3dBase.h"

unsigned short RasterGrid3dBase::convertBlockCoordsToUShort(int in_x, int in_y, int in_z)
{
	int x = in_x * (numberOfTilesPerDimension*numberOfTilesPerDimension);
	int y = in_y * (numberOfTilesPerDimension);
	return x + y + in_z;
}

EnclaveKeyDef::EnclaveKey RasterGrid3dBase::convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert)
{
	return RUtils::convertToRasterGridCell(in_pointToConvert, rCubeDimensionalLength, numberOfTilesPerDimension);
}

float RasterGrid3dBase::getRCubeDimLength()
{
	return rCubeDimensionalLength;
}

float RasterGrid3dBase::getTileDimWeightRatio()
{
	return tileDimWeightToHundredthFloatRatio;
}