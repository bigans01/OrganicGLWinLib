#pragma once

#ifndef RMORPHABLEAREASCANNER_H
#define RMORPHABLEAREASCANNER_H

#include "RMorphableMeshGroup.h"
#include <map>
#include <unordered_map>
#include "Rasterized3DMassGrid.h"
#include "RMorphableMesh.h"
#include "MassGridArrayCellScanArea.h"
#include "DynamicBorderLineList.h"
#include "RPointToGridTranslator.h"
#include "RCollisionPointToPTriangleMapContainer.h"
#include "DebugOption.h"
#include "DebugOptionSet.h"
#include "PolyLogger.h"
#include "SPoly.h"
#include <vector>

class RMorphableAreaScanner
{
	public:
		RMorphableAreaScanner() {};
		template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setScannerDOSpecificRMorphableMesh(EnclaveKeyDef::EnclaveKey in_rMorphableMeshKey,
																														FirstDebugOption && firstOption,
																														RemainingDebugOptions && ...optionParams)
		{
			scannerStoredMeshDebugOptions[in_rMorphableMeshKey] += std::forward<FirstDebugOption>(firstOption);
			setScannerDOSpecificRMorphableMesh(in_rMorphableMeshKey, std::forward<RemainingDebugOptions>(optionParams)...);
		}
		void setScannerDOSpecificRMorphableMesh(EnclaveKeyDef::EnclaveKey in_rMorphableMeshKey) {};

		template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setScannerDOGeneric(FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
		{
			scannerDebugOptions += firstOption;
			handleGenericDO(std::forward<FirstDebugOption>(firstOption));
			setScannerDOGeneric(std::forward<RemainingDebugOptions>(optionParams)...);
		}
		void setScannerDOGeneric() {};

		// setupScanner must be called before adding any SPolys that are converted to RPolys.
		void setupScanner(int in_tilesPerDimension, 
						  float in_dimensionLimit,
			              int in_meshesPerDimension,
			              int in_pointsPerSlicePointArray)		
		{
			pointsPerSlicePointArray = in_pointsPerSlicePointArray;
			scannerCellsPerDimension = in_tilesPerDimension;
			scannerDimLimit = in_dimensionLimit;

			massGrid.setGridParameters(in_tilesPerDimension, in_dimensionLimit);	// Step 1: initialization of grid
			meshesPerDimension = in_meshesPerDimension;
			morphableMeshDimension = scannerDimLimit / meshesPerDimension;	// i.e, 32.0f divided by 8 = 4.0f; also passed as the "thickness" of additive slices.

			scannerDynamicBorderLineList.constructBorders(in_dimensionLimit);

			gridTranslator.setTranslationParameters(scannerCellsPerDimension, scannerDimLimit);
		}

		void addSPolyToGrid(SPoly in_sPolyToAdd);		// Step 2: add all SPolys that will be converted to RPolys, to the grid.
		void buildGridMassShell();						// Step 3: build the mass shell; that is, trace the interior mass of each RTriangle into the massGrid
		void fillGridMass();							// Step 4: fill the mass inside the mass shell
		void scanGridMass();							// Step 5: scan the grid mass, to determine all RMorphableMesh's that have at least one MassGridArrayCell within the mass; 
														// if the "at least one" condition is met, save the RMorphableMesh into a list.
		MassGridArray* fetchMassGridArrayRef();
		std::map<int, RMorphableMeshGroup> meshGroupMap;
		std::vector<SPoly> produceSPolysFromPTriangleMeshes();
	private:
		friend class RMatterCollider;
		void acquireProducedSolutions();
		void handleGenericDO(DebugOption in_debugOption);
		bool checkIfKeysAreNeighbors(EnclaveKeyDef::EnclaveKey in_keyA, EnclaveKeyDef::EnclaveKey in_keyB);
		SPoly convertPTriangleToSPoly(PTriangle in_pTriangle);

		// optional manipulation function calls
		void clampNonFreeMeshPointsToNaturalLimits();

		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> ungroupedMeshes;	// where all the meshes go initially, before doing the grouping pass.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> currentMeshGroup;
		Rasterized3DMassGrid massGrid;
		int scannerCellsPerDimension = 0;
		int pointsPerSlicePointArray = 0;
		float scannerDimLimit = 0.0f;
		DynamicBorderLineList scannerDynamicBorderLineList;

		int meshesPerDimension = 1;		// default value is 1; number of meshes that go in the x/y/z dimension
		float morphableMeshDimension = 0.0f;
		RPointToGridTranslator gridTranslator;
		RCollisionPointToPTriangleMapContainer scannerPointToTriangleMapper;

		DebugOptionSet scannerDebugOptions;

		std::unordered_map<EnclaveKeyDef::EnclaveKey, DebugOptionSet, EnclaveKeyDef::KeyHasher> scannerStoredMeshDebugOptions;	// debug option sets that may be used per RMorphableMesh; 
																																// should be copied to each RMorphableMeshGroup

};		
	

#endif
