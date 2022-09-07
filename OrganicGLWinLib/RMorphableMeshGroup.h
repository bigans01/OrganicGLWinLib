#pragma once

#ifndef RMORPHABLEMESHGROUP_H
#define RMORPHABLEMESHGROUP_H

#include "EnclaveKeyDef.h"
#include "RMorphableMesh.h"
#include <unordered_map>
#include <unordered_set>
#include "RCollisionPointArray.h"
#include "RCollisionPoint.h"
#include "DynamicBorderLineList.h"
#include "IndependentUtils.h"
#include "ECBPPOrientationResults.h"
#include <algorithm>
#include "RAdditiveSliceBase.h"
#include "RAdditiveXSlice.h"
#include "RAdditiveXSliceWestEnd.h"
#include "RAdditiveXSliceEastEnd.h"
#include "RAdditiveXSliceStandalone.h"
#include <mutex>
#include <map>
#include "MassGridArray.h"
#include "RCollisionPointToPTriangleMapContainer.h"
#include "PTriangleMesh.h"
#include "DebugOptionSet.h"
#include "PTriangleMeshPointCalibrator.h"
#include "Operable3DEnclaveKeySet.h"
#include "MassZoneBoxType.h"

class RMorphableMeshGroup
{
	public:
		void insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh);
		void setDynamicBorderRef(DynamicBorderLineList* in_dynamicBorderLineListRef);
		void flagLandlockedMeshes();		// Step 1
		void generatePointArray(int in_slicePointArraySize);					// Step 2
		void flagImmutablePoints();			// Step 3
		void updatePointsWithinMass(MassGridArray* in_massGridArrayRef, RPointToGridTranslator* in_translatorRef);
		Operable3DEnclaveKeySet scanForSolidBlocks(MassGridArray* in_massGridArrayRef);
		void generatePoints();						// Step 4
		bool doesGroupContainKey(EnclaveKeyDef::EnclaveKey in_enclaveKey);
		void printLandlockedPoints();
		void generateRProductFacesInRemainingMeshes();
		void insertMorphableMeshDebugOptions(std::unordered_map<EnclaveKeyDef::EnclaveKey, DebugOptionSet, EnclaveKeyDef::KeyHasher> in_rMorphableMeshDebugOptions);

		// For "Option 1"
		void buildMeshByXScan(MassGridArray* in_massGridArrayRef, float in_sliceThickness, int in_pointsPerSliceArray, RCollisionPointToPTriangleMapContainer* in_pointToTriangleMapContainerRef);

		// For "Option 2"
		void buildMeshByXScanV2(MassGridArray* in_massGridArrayRef, 
								float in_sliceThickness, 
								int in_pointsPerSliceArray, 
								RCollisionPointToPTriangleMapContainer* in_pointToTriangleMapContainerRef,
								float in_cubeDimLength,
								float in_tileDimWeightRatio,
								int in_tilesPerDim
								);
		void produceSolution(MassGridArray* in_massGridArrayRef,
			float in_tileGridWidth,
			int in_numberOfTilesPerDimension);

		// optional manipulation functions; these two functions should be called together in RMorphableAreaScanner::clampNonFreeMeshPointsToNaturalLimits(),
		// with calibrate* below called first.
		void calibratePTriangleMeshPoints(RPointToGridTranslator* in_translatorRef, MassZoneBoxType in_boxTypeForCalibrator);
		void determineGroupMeshBoundaries(MassZoneBoxType in_boundaryType);

		PTriangleMesh groupMesh;

	private:
		friend class RMorphableAreaScanner;

		void determineBestPointCount(int in_potentialPointCount);
		bool checkForMorphableMeshDebugOption(EnclaveKeyDef::EnclaveKey in_morphableMeshKey, DebugOption in_optionToCheck);

		int slicePointArraySize = 0;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> keyedMorphables;
		RCollisionPointArray meshGroupPointArray;
		DynamicBorderLineList* dynamicBorderRef = nullptr;

		std::map<int, std::unique_ptr<RAdditiveSliceBase>> sliceMap;	// stores the slices produced during the mesh group's chosen scan function
		int setPointCount = 0;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, DebugOptionSet, EnclaveKeyDef::KeyHasher> rMorphableMeshDebugOptions;

};

#endif
