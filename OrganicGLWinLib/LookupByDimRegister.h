#pragma once

#ifndef LOOKUPBYDIMREGISTER_H
#define LOOKUPBYDIMREGISTER_H

#include <unordered_set>
#include "EnclaveKeyDef.h"
#include "MassGridArray.h"
#include "RTriangleInteriorAreaTracer.h"
#include "RScanDim.h"
#include <glm/glm.hpp>

class LookupByDimRegister
{
public:
	LookupByDimRegister() {};
	LookupByDimRegister(int in_dimValue, RScanDim in_scanDimension) :
		dimValue(in_dimValue),
		scanDimension(in_scanDimension)
	{};
private:
	friend class RTriangle;
	struct Settable2DRef
	{
		Settable2DRef() {};
		Settable2DRef(bool in_wasSet,
			std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_ref) :
			wasSet(in_wasSet),
			ref(in_ref)
		{};
		bool wasSet = false;
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* ref = nullptr;
	};

	struct TwoDKeyPair
	{
		TwoDKeyPair() {};
		TwoDKeyPair(EnclaveKeyDef::Enclave2DKey in_keyA, EnclaveKeyDef::Enclave2DKey in_keyB) :
			twoDKeyA(in_keyA),
			twoDKeyB(in_keyB)
		{};

		EnclaveKeyDef::Enclave2DKey twoDKeyA;
		EnclaveKeyDef::Enclave2DKey twoDKeyB;
	};

	int dimValue = 0;	// x,y,z or coordinate value that represents the slice to scan. 
						// i.e, dimValue = 3 could mean:
						// scan y and z at x = 3
						// scan x and z at y = 3
						// scan x and y at z = 3		
	RScanDim scanDimension = RScanDim::NOVAL;
	int numberOfInvolvedLines = 0;
	Settable2DRef lineSetRefs[3];
	std::vector<TwoDKeyPair> scanRuns;

	void insertLineSetRef(int in_lineID,
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_lineSetRef);
	void buildScanRuns();
	void executeScanRuns(MassGridArray* in_massGridArrayRef,
		float in_rPolyRCubeDimLength,
		float in_rPolyTilesPerDim,
		float in_rPolyTileWeightToHundredthFloatRatio,
		short in_downfillCrustBitValue,
		short in_upfillCrustBitValue, glm::vec3 in_emptyNormal);
};
#endif