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

class RMorphableMeshGroup
{
	public:
		void insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh);
		void setDynamicBorderRef(DynamicBorderLineList* in_dynamicBorderLineListRef);
		void flagLandlockedMeshes();		// Step 1
		void generatePointArray(int in_slicePointArraySize);					// Step 2
		void updatePointLandlockStats();			// Step 3
		void generatePoints();						// Step 4
		bool doesGroupContainKey(EnclaveKeyDef::EnclaveKey in_enclaveKey);
		void printLandlockedPoints();
		void generateRProductFacesInRemainingMeshes();
		void buildMeshByXScan(MassGridArray* in_massGridArrayRef, float in_sliceThickness, int in_pointsPerSliceArray);
	private:
		friend class RMorphableAreaScanner;
		int slicePointArraySize = 0;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> keyedMorphables;
		RCollisionPointArray meshGroupPointArray;
		DynamicBorderLineList* dynamicBorderRef = nullptr;

		std::map<int, std::unique_ptr<RAdditiveSliceBase>> sliceMap;	// stores the slices produced during the mesh group's chosen scan function
};

#endif
