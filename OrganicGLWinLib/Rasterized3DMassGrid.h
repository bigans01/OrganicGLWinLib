#pragma once

#ifndef RASTERIZED3DMASSGRID_H
#define RASTERIZED3DMASSGRID_H

#include "RasterGrid3dBase.h"
#include <map>
#include "RPoly.h"
#include "MassGridArray.h"

class Rasterized3DMassGrid : public RasterGrid3dBase
{
	public:	
		Rasterized3DMassGrid() {};
		void setGridParameters(int in_tilesPerDimension, float in_dimensionLimit)
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

			dataArray.buildArray(in_tilesPerDimension);
		};
		void addGridRPoly(SPoly in_sPolyToResolve);
		void buildShell();				// the shell that must be constructed, so that we can fill the mass inside of the shell.
		void fillMass();				// fills the mass of the grid; must be called only after buildShell() has been run.
	private:
		std::map<int, RPoly> rPolyMap;
		int gridRPolyCount = 0;
		MassGridArray dataArray;
};
#endif
