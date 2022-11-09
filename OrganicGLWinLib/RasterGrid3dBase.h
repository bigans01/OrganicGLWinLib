#pragma once

#ifndef RASTERGRID3DBASE_H
#define RASTERGRID3DBASE_H

#include "SPoly.h"
#include "RUtils.h"
#include "RPoly.h"

class RasterGrid3dBase
{
	public:
		unsigned short convertBlockCoordsToUShort(int in_x, int in_y, int in_z);
		EnclaveKeyDef::EnclaveKey convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert);
		float getRCubeDimLength();
		float getTileDimWeightRatio();
	protected:
		int numberOfTilesPerDimension = 0;
		float dimensionLimit = 0.0f;
		float rCubeDimensionalLength = 0.0f;
		float tileDimWeightToHundredthFloatRatio = 0.0f;
};

#endif
