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

class RMorphableMeshGroup
{
	public:
		void insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh);
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> keyedMorphables;
		void setDynamicBorderRef(DynamicBorderLineList* in_dynamicBorderLineListRef);
		void removeInteriorLandlockedMeshes();		// Step 1
		void generatePointArray();					// Step 2
		void generatePoints();						// Step 3
	private:
		RCollisionPointArray meshGroupPointArray;
		DynamicBorderLineList* dynamicBorderRef = nullptr;
};

#endif
