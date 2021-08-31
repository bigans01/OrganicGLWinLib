#pragma once

#ifndef PTRIANGLEMESHPOINTCALIBRATOR_H
#define PTRIANGLEMESHPOINTCALIBRATOR_H

#include "RPointToGridTranslator.h"
#include "RCollisionPointArray.h"
#include "DynamicRasterGridCellBorders.h"
#include <iomanip>
#include "IndependentUtils.h"
#include <glm/glm.hpp>

class PTriangleMeshPointCalibrator
{
	public:
		PTriangleMeshPointCalibrator(RCollisionPointArray* in_translatorPointArrayRef,
									 RPointToGridTranslator* in_translatorGridRef) 
		{
			translatorPointArrayRef = in_translatorPointArrayRef;
			translatorGridRef = in_translatorGridRef;

			determineCalibrationGridLimit(in_translatorGridRef->dimensionLimit, 
										  in_translatorGridRef->dimPerTile, 
										  in_translatorGridRef->numberOfTilesPerDimension);
			//std::cout << std::setprecision(8) << std::endl;

			std::cout << ">>>> min limit: " << minCalibrationGridLimit << std::endl;
			std::cout << ">>>> max limit: " << maxCalibrationGridLimit << std::endl;

			dynamicGridCellBorders.constructBorders(minCalibrationGridLimit, maxCalibrationGridLimit);
			calibratePointsInArray();
		};
	private:
		void determineCalibrationGridLimit(float in_dimLimit, float in_cellLength, int in_cellsPerDimension);
		void calibratePointsInArray();
		void calibratePoint(RCollisionPoint* in_collisionPointRef);
		void applyCalibration(RCollisionPoint* in_collisionPointRef, MassGridArrayCellLimit in_cellLimit);
		
		RCollisionPointArray* translatorPointArrayRef = nullptr;
		RPointToGridTranslator* translatorGridRef = nullptr;
		DynamicRasterGridCellBorders dynamicGridCellBorders;

		float maxCalibrationGridLimit = 0.0f;	// the raster grid point x,y, or z value that represents the "edge" or "limits" of the calibration array.
												// i.e, if the limit is 4.0f, and there are 40 cells per dimension, each cell measures a width of 1.0f. 
												// The center point of the last cell at index 39, 39, 39 is actually 3.95, 3.95, 3.95.
												//
												// Logic: (index * cellLength) + (cellLength / 2)
		float minCalibrationGridLimit = 0.0f;
};

#endif