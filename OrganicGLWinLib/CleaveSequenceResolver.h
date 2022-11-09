#pragma once

#ifndef CLEAVESEQUENCERESOLVER_H
#define CLEAVESEQUENCERESOLVER_H

#include "SPoly.h"
#include "RUtils.h"
#include "RPoly.h"
#include "RasterGrid3dBase.h"

class CleaveSequenceResolver : public RasterGrid3dBase
{
	public:
		CleaveSequenceResolver() {};
		CleaveSequenceResolver(int in_tilesPerDimension, float in_dimensionLimit)
		{
			dimensionLimit = in_dimensionLimit;
			numberOfTilesPerDimension = in_tilesPerDimension;
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
	private:
		RPoly rPolyToResolve;

};

#endif
