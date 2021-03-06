#pragma once

#ifndef RUTILS_H
#define RUTILS_H

#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>

class RUtils
{
	public:
		static EnclaveKeyDef::EnclaveKey convertToRasterGridCell(glm::vec3 in_pointToConvert, float in_tileGridWidth, int in_numberOfTilesPerDimension);
		static glm::vec3 convertToRasterGridPoint(glm::vec3 in_pointToConvert, 
											      float in_tileDimensionalWeight, 
												  int in_numberOfTilesPerDimension,
												  float in_tileDimWeightToHundredthRatio);
};

#endif
