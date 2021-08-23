#pragma once

#include "ECBPolyPoint.h"

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
		ECBPolyPoint  corner_UpperNW;
		ECBPolyPoint  corner_UpperSW;
		ECBPolyPoint  corner_UpperSE;
		ECBPolyPoint  corner_UpperNE;

		ECBPolyPoint  corner_LowerNW;
		ECBPolyPoint  corner_LowerSW;
		ECBPolyPoint  corner_LowerSE;
		ECBPolyPoint  corner_LowerNE;

		// Type 2 borders (lines)
		ECBPolyPoint Zaxis_lowerWest;
		ECBPolyPoint Zaxis_lowerEast;
		ECBPolyPoint Zaxis_upperWest;
		ECBPolyPoint Zaxis_upperEast;

		ECBPolyPoint Xaxis_lowerNorth;
		ECBPolyPoint Xaxis_lowerSouth;
		ECBPolyPoint Xaxis_upperNorth;
		ECBPolyPoint Xaxis_upperSouth;

		ECBPolyPoint Yaxis_northWest;
		ECBPolyPoint Yaxis_northEast;
		ECBPolyPoint Yaxis_southWest;
		ECBPolyPoint Yaxis_southEast;

		// Type 1 borders (faces)
		ECBPolyPoint WestFace;
		ECBPolyPoint NorthFace;
		ECBPolyPoint EastFace;
		ECBPolyPoint SouthFace;
		ECBPolyPoint TopFace;
		ECBPolyPoint BottomFace;

	private:

		void setRasterGridCellBorders();

		float minBorder = 0.0f;
		float maxBorder = 0.0f;

};

#endif
