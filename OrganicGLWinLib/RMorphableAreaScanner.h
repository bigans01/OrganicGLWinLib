#pragma once

#ifndef RMORPHABLEAREASCANNER_H
#define RMORPHABLEAREASCANNER_H

#include "RMorphableMeshGroup.h"
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
#include "Operable3DEnclaveKeySet.h"
#include "MassZoneBoxType.h"

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

		template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setDOSpecificRPoly(int in_rPolyID, FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
		{
			setSpecificRPolyOption(in_rPolyID, std::forward<FirstDebugOption>(firstOption));
			setDOSpecificRPoly(in_rPolyID, std::forward<RemainingDebugOptions>(optionParams)...);
		}
		void setDOSpecificRPoly(int in_rPolyID) {};

		// Step 1: setupScanner must be called before adding any SPolys that are converted to RPolys.
		void setupScanner(int in_tilesPerDimension,
			float in_dimensionLimit,
			int in_meshesPerDimension,
			int in_pointsPerSlicePointArray,
			MassZoneBoxType in_massZoneBoxType);

		void addSPolyToGrid(SPoly in_sPolyToAdd);		// Step 2: add all SPolys that will be converted to RPolys, to the grid.
		void buildGridMassShell();						// Step 3: build the mass shell; that is, trace the interior mass of each RTriangle into the massGrid
		void fillGridMass();							// Step 4: fill the mass inside the mass shell
		void scanGridMass();							// Step 5: scan the grid mass, to determine all RMorphableMesh's that have at least one MassGridArrayCell within the mass; 
														// if the "at least one" condition is met, save the RMorphableMesh into a list.
		bool checkIfKeyedMorphableMeshExistsAnywhere(EnclaveKeyDef::EnclaveKey in_keyToSearch);

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

		void setSpecificRPolyOption(int in_rPolyID, DebugOption in_debugOption);

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
		Operable3DEnclaveKeySet wholeBlocks;				// contains a list of blocks from an RMatter operation that are "whole", as in, their entire mass is surrounded.
		Operable3DEnclaveKeySet	wholeBlocksWithFilling;		// contains a list of blocks from an RMatter operation that are "whole", but also had MassCellBitFlags::INNER_MASS set;
															// needed by the ORE function, setPendingRMatterSolids

		DebugOptionSet scannerDebugOptions;
		MassZoneBoxType selectedBoxType = MassZoneBoxType::NOVAL;	// needs to be used when determining if we will be populationg wholeBlocks,
																	// which shoudl only be done in when it is set to MassZoneBoxType::ENCLAVE

		std::unordered_map<EnclaveKeyDef::EnclaveKey, DebugOptionSet, EnclaveKeyDef::KeyHasher> scannerStoredMeshDebugOptions;	// debug option sets that may be used per RMorphableMesh; 
																																// should be copied to each RMorphableMeshGroup

};		
	

#endif
