#pragma once

#ifndef PTRIANGLEMESHPOINTCALIBRATOR_H
#define PTRIANGLEMESHPOINTCALIBRATOR_H

#include "RPointToGridTranslator.h"
#include "RCollisionPointArray.h"

class PTriangleMeshPointCalibrator
{
	public:
		PTriangleMeshPointCalibrator(RCollisionPointArray* in_translatorPointArrayRef,
									 RPointToGridTranslator* in_translatorGridRef) 
		{
			determineCalibrationGridLimit(in_translatorGridRef->dimensionLimit, 
										  in_translatorGridRef->dimPerTile, 
										  in_translatorGridRef->numberOfTilesPerDimension);
			std::cout << ">>>> min limit: " << minCalibrationGridLimit << std::endl;
			std::cout << ">>>> max limit: " << maxCalibrationGridLimit << std::endl;
		};
	private:
		void determineCalibrationGridLimit(float in_dimLimit, float in_cellLength, int in_cellsPerDimension);

		RCollisionPointArray* translatorPointArrayRef = nullptr;
		RPointToGridTranslator* translatorGridRef = nullptr;
		float maxCalibrationGridLimit = 0.0f;	// the raster grid point x,y, or z value that represents the "edge" or "limits" of the calibration array.
												// i.e, if the limit is 4.0f, and there are 40 cells per dimension, each cell measures a width of 1.0f. 
												// The center point of the last cell at index 39, 39, 39 is actually 3.95, 3.95, 3.95.
												//
												// Logic: (index * cellLength) + (cellLength / 2)
		float minCalibrationGridLimit = 0.0f;
};

#endif
