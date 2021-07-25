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

class RMorphableMeshGroup
{
	public:
		void insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh);
		void setDynamicBorderRef(DynamicBorderLineList* in_dynamicBorderLineListRef);
		void removeInteriorLandlockedMeshes();		// Step 1
		void generatePointArray();					// Step 2
		void updatePointLandlockStats();			// Step 3
		void generatePoints();						// Step 4
		bool doesGroupContainKey(EnclaveKeyDef::EnclaveKey in_enclaveKey);
		void printLandlockedPoints();
		void generateRProductFacesInRemainingMeshes();
		void buildMeshByXScan();
	private:
		friend class RMorphableAreaScanner;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> keyedMorphables;
		RCollisionPointArray meshGroupPointArray;
		DynamicBorderLineList* dynamicBorderRef = nullptr;
};

#endif
