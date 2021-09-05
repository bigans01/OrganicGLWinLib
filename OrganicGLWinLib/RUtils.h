#pragma once

#ifndef RUTILS_H
#define RUTILS_H

#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>

class RUtils
{
	public:
		static EnclaveKeyDef::EnclaveKey convertToRasterGridCell(glm::vec3 in_pointToConvert, float in_tileGridWidth, int in_numberOfTilesPerDimension);
		static glm::vec3 convertGridCellLocationToVec3(int in_locationX, int in_locationY, int in_locationZ, float in_cubeDimLength);
		static glm::vec3 convertToRasterGridPoint(glm::vec3 in_pointToConvert, 
											      float in_tileDimensionalWeight, 
												  int in_numberOfTilesPerDimension,
												  float in_tileDimWeightToHundredthRatio);
		static EnclaveKeyDef::EnclaveKey findRasterGridCellCentroid(glm::vec3 in_point0, 
																	glm::vec3 in_point1, 
																	glm::vec3 in_point2, 
																	float in_tileGridWidth, 
																	int in_numberOfTilesPerDimension);
};

#endif
