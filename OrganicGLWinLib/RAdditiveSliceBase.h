#pragma once

#ifndef RADDITIVESLICEBASE_H
#define RADDITIVESLICEBASE_H

#include "RAdditiveSliceType.h"
#include <mutex>
#include "RCollisionPoint.h"
#include "MassGridArray.h"
#include "RCollisionPointArray.h"
#include "IndependentUtils.h"
#include <unordered_map>
#include "RMorphableMesh.h"
#include <algorithm>
#include <iostream>
#include "DynamicBorderLineList.h"

class RAdditiveSliceBase
{
	public:
		// virtual functions
		virtual void buildPointSets() = 0;

		// non virtual functions
		void initialize(RAdditiveSliceType in_sliceType, 
						int in_numberofTypicalSetPoints, 
						MassGridArray* in_gridArrayRef, 
						RCollisionPointArray* in_pointArrayRef, 
						float in_sliceThickness,
						int in_sliceIndexID,
						std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher>* in_keyedMorphablesRef,
						std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_sliceMorphableKeys,
			            DynamicBorderLineList* in_sliceDynBorderRef);
		void copySetBRefsFromPreviousSlice(RAdditiveSliceBase* in_otherSliceRef);
		void printAllSetPoints();

	protected:
		RAdditiveSliceType sliceType = RAdditiveSliceType::NOVAL;
		int numberOfPointsPerTypicalSet = 0;
		float sliceThickness; // how "thick" a slice is, measured in float.
		int sliceIndex = 0;	// the slice index
		bool areSetsInitialized = false;
		std::unique_ptr<RCollisionPoint*[]> typicalPointSetA;
		std::unique_ptr<RCollisionPoint*[]> typicalPointSetB;
		MassGridArray* gridArrayRef = nullptr;
		RCollisionPointArray*  pointArrayRef = nullptr;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher>* keyedMorphablesRef = nullptr;
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> sliceMorphableKeys;
		DynamicBorderLineList* sliceDynBorderRef = nullptr;
};

#endif
