#include "stdafx.h"
#include "RAdditiveSliceBase.h"

void RAdditiveSliceBase::initialize(RAdditiveSliceType in_sliceType, 
									int in_numberofTypicalSetPoints, 
									MassGridArray* in_gridArrayRef, 
									RCollisionPointArray* in_pointArrayRef,
									float in_sliceThickness,
									int in_sliceIndexID, 
									std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher>* in_keyedMorphablesRef,
									std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> in_sliceMorphableKeys,
									DynamicBorderLineList* in_sliceDynBorderRef)
{
	sliceType = in_sliceType;
	numberOfPointsPerTypicalSet = in_numberofTypicalSetPoints;
	areSetsInitialized = true;
	typicalPointSetA.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
	typicalPointSetB.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
	gridArrayRef = in_gridArrayRef;
	pointArrayRef = in_pointArrayRef;
	sliceThickness = in_sliceThickness;
	sliceIndex = in_sliceIndexID;
	keyedMorphablesRef = in_keyedMorphablesRef;
	sliceMorphableKeys = in_sliceMorphableKeys;
	sliceDynBorderRef = in_sliceDynBorderRef;
}

void RAdditiveSliceBase::copySetBRefsFromPreviousSlice(RAdditiveSliceBase* in_otherSliceRef)
{
	// copy, only if this instance and the other instance have their sets initialized
	if
	(
		(areSetsInitialized == true)
		&&
		(in_otherSliceRef->areSetsInitialized == true)
	)
	{
		for (int x = 0; x < numberOfPointsPerTypicalSet; x++)
		{
			// copy B into A
			typicalPointSetA[x] = in_otherSliceRef->typicalPointSetB[x];
		}
	}
}

void RAdditiveSliceBase::printAllSetPoints()
{
	if
	(
		(areSetsInitialized == true)
	)
	{
		// print set A
		std::cout << ":::: Set A points: " << std::endl;
		for (int x = 0; x < numberOfPointsPerTypicalSet; x++)
		{
			std::cout << x << ": " << typicalPointSetA[x]->originalValue.x << ", " << typicalPointSetA[x]->originalValue.y << ", " << typicalPointSetA[x]->originalValue.z << std::endl;
		}

		std::cout << ":::: Set B points: " << std::endl;
		for (int x = 0; x < numberOfPointsPerTypicalSet; x++)
		{
			std::cout << x << ": " << typicalPointSetB[x]->originalValue.x << ", " << typicalPointSetB[x]->originalValue.y << ", " << typicalPointSetB[x]->originalValue.z << std::endl;
		}
	}
}