#pragma once

#ifndef PTRIANGLEMESHPOINTCALIBRATOR_H
#define PTRIANGLEMESHPOINTCALIBRATOR_H

#include "RPointToGridTranslator.h"
#include "RCollisionPointArray.h"
#include "DynamicRasterGridCellBorders.h"
#include <iomanip>
#include "MassZoneBoxType.h"

class PTriangleMeshPointCalibrator
{
	public:
		PTriangleMeshPointCalibrator(RCollisionPointArray* in_translatorPointArrayRef,
									 RPointToGridTranslator* in_translatorGridRef,
								     MassZoneBoxType in_boxTypeForMaxXYZ) 
		{
			translatorPointArrayRef = in_translatorPointArrayRef;
			translatorGridRef = in_translatorGridRef;

			// set appropriate maxXYZ value
			switch (in_boxTypeForMaxXYZ)
			{
				case (MassZoneBoxType::BLOCK): { maxXYZ = 1.0f; break;}
				case (MassZoneBoxType::ENCLAVE): { maxXYZ = 4.0f; break;}
				case (MassZoneBoxType::COLLECTION): { maxXYZ = 32.0f; break;}
			}

			determineCalibrationGridLimit(in_translatorGridRef->dimensionLimit, 
										  in_translatorGridRef->dimPerTile, 
										  in_translatorGridRef->numberOfTilesPerDimension);
			//std::cout << std::setprecision(8) << std::endl;

			//std::cout << ">>>> min limit: " << minCalibrationGridLimit << std::endl;
			//std::cout << ">>>> max limit: " << maxCalibrationGridLimit << std::endl;

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

		float maxXYZ = 0.0f;	// the value that would be used when calibrating points that are on the "max"; 
								// i.e., if working with blocks, then this is 1.0f, if ORE, then 4.0f, and if blueprint, then 32.0f.
								// this value should be set in the constructor.
};

#endif
