#pragma once

#ifndef CLEAVESEQUENCERESOLVER_H
#define CLEAVESEQUENCERESOLVER_H

#include "EnclaveKeyDef.h"
#include <iostream>
#include "SPoly.h"
#include "RUtils.h"
#include "RPoly.h"

class CleaveSequenceResolver
{
	public:
		CleaveSequenceResolver() {};
		CleaveSequenceResolver(int in_tilesPerDimension, float in_dimensionLimit) :
			numberOfTilesPerDimension(in_tilesPerDimension),
			dimensionLimit(in_dimensionLimit)
		{
			rCubeDimensionalLength = dimensionLimit / numberOfTilesPerDimension;	//  i.e., for Enclave resolution, this would be:
																				// dimensionLimit = 4.0f
																				// numberOfTilesPerDiemnsion = 400
																				// tileGridWith = 0.01f	
			std::cout << "Rcube dimensional length: " << rCubeDimensionalLength << std::endl;
			tileDimWeightToHundredthFloatRatio = 100.0f / (numberOfTilesPerDimension / dimensionLimit);
			std::cout << "tile weight to hundredth float ratio: " << tileDimWeightToHundredthFloatRatio << std::endl;
		};

		void addRPolyToResolve(SPoly in_sPolyToResolve);
		void addRPolyToCompare(SPoly in_comparingSPoly);
		void printRPolyToResolvePoints();
		EnclaveKeyDef::EnclaveKey convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert);

	private:
		int numberOfTilesPerDimension = 0;
		float dimensionLimit = 0.0f;
		float rCubeDimensionalLength = 0.0f;
		float tileDimWeightToHundredthFloatRatio = 0.0f;
		unsigned short convertBlockCoordsToUShort(int in_x, int in_y, int in_z);

		RPoly rPolyToResolve;

};

#endif
