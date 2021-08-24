#pragma once

#include "ECBPolyPoint.h"
#include "MassGridArrayCellLimit.h"

#ifndef DYNAMICRASTERGRIDCELLBORDERS_H
#define DYNAMICRASTERGRIDCELLBORDERS_H

class DynamicRasterGridCellBorders
{
	public:
		DynamicRasterGridCellBorders() {};
		void constructBorders(float in_minBorder, float in_maxBorder)
		{
			minBorder = in_minBorder;
			maxBorder = in_maxBorder;
			setRasterGridCellBorders();
		}

		// Type 1 borders (corners)
		MassGridArrayCellLimit  corner_UpperNW;
		MassGridArrayCellLimit  corner_UpperSW;
		MassGridArrayCellLimit  corner_UpperSE;
		MassGridArrayCellLimit  corner_UpperNE;
		
		MassGridArrayCellLimit  corner_LowerNW;
		MassGridArrayCellLimit  corner_LowerSW;
		MassGridArrayCellLimit  corner_LowerSE;
		MassGridArrayCellLimit  corner_LowerNE;
		
		// Type 2 borders (lines)
		MassGridArrayCellLimit Zaxis_lowerWest;
		MassGridArrayCellLimit Zaxis_lowerEast;
		MassGridArrayCellLimit Zaxis_upperWest;
		MassGridArrayCellLimit Zaxis_upperEast;
		
		MassGridArrayCellLimit Xaxis_lowerNorth;
		MassGridArrayCellLimit Xaxis_lowerSouth;
		MassGridArrayCellLimit Xaxis_upperNorth;
		MassGridArrayCellLimit Xaxis_upperSouth;
		
		MassGridArrayCellLimit Yaxis_northWest;
		MassGridArrayCellLimit Yaxis_northEast;
		MassGridArrayCellLimit Yaxis_southWest;
		MassGridArrayCellLimit Yaxis_southEast;
		
		// Type 3 borders (faces)
		MassGridArrayCellLimit WestFace;
		MassGridArrayCellLimit NorthFace;
		MassGridArrayCellLimit EastFace;
		MassGridArrayCellLimit SouthFace;
		MassGridArrayCellLimit TopFace;
		MassGridArrayCellLimit BottomFace;

	private:
		void setRasterGridCellBorders();

		float minBorder = 0.0f;
		float maxBorder = 0.0f;

};

#endif
