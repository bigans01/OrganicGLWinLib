#pragma once

#ifndef LOOKUPBYDIMREGISTER_H
#define LOOKUPBYDIMREGISTER_H

#include <unordered_set>
#include "EnclaveKeyDef.h"
#include "MassGridArray.h"
#include "RTriangleInteriorAreaTracer.h"

class LookupByDimRegister
{
	public:
		LookupByDimRegister() {};
		LookupByDimRegister(int in_dimValue) :
			dimValue(in_dimValue)
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
			TwoDKeyPair(EnclaveKeyDef::EnclaveKey in_keyA, EnclaveKeyDef::EnclaveKey in_keyB) :
				twoDKeyA(in_keyA),
				twoDKeyB(in_keyB)
			{};

			EnclaveKeyDef::EnclaveKey twoDKeyA;
			EnclaveKeyDef::EnclaveKey twoDKeyB;
		};

		int dimValue = 0;	// x,y,z or coordinate value that represents the slice to scan. 
		                    // i.e, dimValue = 3 could mean:
							// scan y and z at x = 3
							// scan x and z at y = 3
							// scan x and y at z = 3		
		int numberOfInvolvedLines = 0;

		Settable2DRef lineSetRefs[3];
		void insertLineSetRef(int in_lineID,
							  std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_lineSetRef)
		{
			Settable2DRef newRef(true, in_lineSetRef);
			lineSetRefs[in_lineID] = newRef;
		}

		void buildScanRuns()
		{
			for (int x = 0; x < 3; x++)
			{
				if (lineSetRefs[x].wasSet == true)
				{
					numberOfInvolvedLines++;
				}
			}

			// run logic for two involved lines
			if (numberOfInvolvedLines == 2)
			{

			}
			// .... three lines
			else if (numberOfInvolvedLines == 3)
			{

			}

		};

		void executeScanRuns(MassGridArray* in_massGridArrayRef,
			float in_rPolyRCubeDimLength,
			float in_rPolyTilesPerDim,
			float in_rPolyTileWeightToHundredthFloatRatio)
		{
			// use the new class derived from RasterCubeTracerBase.h here 
			RTriangleInteriorAreaTracer interiorTracer;
			interiorTracer.setGridArrayRef(in_massGridArrayRef);
		}
};

#endif