#pragma once

#ifndef RASTERIZED3DMASSGRID_H
#define RASTERIZED3DMASSGRID_H

#include "RasterGrid3dBase.h"
#include "RPoly.h"
#include "MassGridArray.h"
#include "MassGridArrayCellScanArea.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "DebugOption.h"
#include "DebugOptionSet.h"

class Rasterized3DMassGrid : public RasterGrid3dBase
{
	public:	
		Rasterized3DMassGrid() {};
	private:
		friend class RMorphableMesh;
		friend class RMorphableAreaScanner;
		friend class RMatterCollider;

		void insertSpecificRPolyDO(int in_rPolyID, DebugOption in_debugOption);
		void appendMatterFromOtherArray(MassGridArray* in_otherDataArrayRef);
		void appendAntiMatterFromOtherArray(MassGridArray* in_otherDataArrayRef);
		void addGridRPoly(SPoly in_sPolyToResolve);
		void buildShell();		// the shell that must be constructed, so that we can fill the mass inside of the shell.
		void fillMass();		// fills the mass of the grid; must be called only after buildShell() has been run.
		void setGridParameters(int in_tilesPerDimension, float in_dimensionLimit);	// parameters to set before any RPolys are added
		void setLoggerDebugLevel(PolyDebugLevel in_logLevel);	// optional debug output
		bool wasMassFoundInAreaScan(MassGridArrayCellScanArea in_scanArea);		
		void printRPolyTriangles();
		MassGridSearchResult fetchCell(EnclaveKeyDef::EnclaveKey in_key);
		MassGridArray* fetchDataArrayRef();

		std::map<int, RPoly> rPolyMap;
		std::map<int, DebugOptionSet> rPolyDebugOptionSets;
		int gridRPolyCount = 0;
		MassGridArray dataArray;
		PolyDebugLevel massGridLogLevel = PolyDebugLevel::NONE;
		PolyLogger massGridLogger;
};
#endif
