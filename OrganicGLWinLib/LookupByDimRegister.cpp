#include "stdafx.h"
#include "LookupByDimRegister.h"

void LookupByDimRegister::insertLineSetRef(int in_lineID,
	std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_lineSetRef)
{
	Settable2DRef newRef(true, in_lineSetRef);
	lineSetRefs[in_lineID] = newRef;
}

void LookupByDimRegister::printScanRunKeys()
{
	auto scansBegin = scanRuns.begin();
	auto scansEnd = scanRuns.end();
	int currentScan = 0;
	for (; scansBegin != scansEnd; scansBegin++)
	{
		std::cout << "Scan " << currentScan << ":" << std::endl;

		switch (scanDimension)
		{
			case RScanDim::X:
			{
				std::cout << "Key A: (" << dimValue << ", " << scansBegin->twoDKeyA.a << ", " << scansBegin->twoDKeyA.b << ") " << std::endl;
				std::cout << "Key B: (" << dimValue << ", " << scansBegin->twoDKeyB.a << ", " << scansBegin->twoDKeyB.b << ") " << std::endl;
				break;
			};

			case RScanDim::Y:
			{
				std::cout << "Key A: (" << scansBegin->twoDKeyA.a << ", " << dimValue << ", " << scansBegin->twoDKeyA.b << ") " << std::endl;
				std::cout << "Key B: (" << scansBegin->twoDKeyB.a << ", " << dimValue << ", " << scansBegin->twoDKeyB.b << ") " << std::endl;
				break;
			}

			case RScanDim::Z:
			{
				std::cout << "Key A: (" << scansBegin->twoDKeyA.a << ", " << scansBegin->twoDKeyA.b << ", " << dimValue << ") " << std::endl;
				std::cout << "Key B: (" << scansBegin->twoDKeyB.a << ", " << scansBegin->twoDKeyB.b << ", " << dimValue << ") " << std::endl;
				break;
			}
		}

		currentScan++;
	}
}

void LookupByDimRegister::buildScanRuns(bool in_debugFlag)
{
	int involvedLineIndexArray[3];
	int currentInvolvedLineIndex = 0;

	for (int x = 0; x < 3; x++)
	{
		if (lineSetRefs[x].wasSet == true)
		{
			numberOfInvolvedLines++;
			involvedLineIndexArray[currentInvolvedLineIndex++] = x;
		}
	}

	if (in_debugFlag == true)
	{
		std::cout << "!! Number of involved lines for this scan run: " << numberOfInvolvedLines << std::endl;
	}

	// run logic for two involved lines
	if (numberOfInvolvedLines == 2)
	{
		// only one "style" of interior trace to run, since there are two lines; 
		Settable2DRef* lineARef = &lineSetRefs[involvedLineIndexArray[0]];
		Settable2DRef* lineBRef = &lineSetRefs[involvedLineIndexArray[1]];

		// all keys of line A, must be run against all keys of line B
		auto lineASetBegin = lineARef->ref->begin();
		auto lineASetEnd = lineARef->ref->end();
		for (; lineASetBegin != lineASetEnd; lineASetBegin++)
		{
			EnclaveKeyDef::Enclave2DKey currentLineAKey = *lineASetBegin;
			auto lineBSetBegin = lineBRef->ref->begin();
			auto lineBSetEnd = lineBRef->ref->end();
			for (; lineBSetBegin != lineBSetEnd; lineBSetBegin++)
			{
				EnclaveKeyDef::Enclave2DKey currentLineBKey = *lineBSetBegin;
				TwoDKeyPair currentScanKeys(currentLineAKey, currentLineBKey);
				scanRuns.push_back(currentScanKeys);
			}
		}

	}
	// .... three lines
	else if (numberOfInvolvedLines == 3)
	{
		if (in_debugFlag == true)
		{
			std::cout << "!!!! Notice: 3 involved lines detected during build of scan run..... " << std::endl;
		}
	}

}

void LookupByDimRegister::executeScanRuns(MassGridArray* in_massGridArrayRef,
	float in_rPolyRCubeDimLength,
	float in_rPolyTilesPerDim,
	float in_rPolyTileWeightToHundredthFloatRatio,
	short in_downfillCrustBitValue,
	short in_upfillCrustBitValue, 
	glm::vec3 in_emptyNormal,
	TriangleMaterial in_scanRunMaterialID)
{
	int totalScansRan = 0;
	auto scansBegin = scanRuns.begin();
	auto scansEnd = scanRuns.end();
	for (; scansBegin != scansEnd; scansBegin++)
	{
		//std::cout << "---> Running scan... for key: " << std::endl;
		/*
		if
		(
			(scansBegin->twoDKeyA.a < 0)
			||
			(scansBegin->twoDKeyA.b < 0)
			||
			(scansBegin->twoDKeyB.a < 0)
			|| 
			(scansBegin->twoDKeyB.b < 0)
		)
		{
			std::cout << "!!! Warning, value < 0 detected! " << std::endl;
		}
		*/

		// use the new class derived from RasterCubeTracerBase.h here 
		//RTriangleInteriorAreaTracer interiorTracer;
		//interiorTracer.setOptionalMassGridArrayRef(in_massGridArrayRef);
		//interiorTracer.setGridArrayRef(in_massGridArrayRef);

		//EnclaveKeyDef::EnclaveKey startKey;
		//EnclaveKeyDef::EnclaveKey endKey;

		BrasenhamMassTracerDim selectedDim = BrasenhamMassTracerDim::NOVAL;

		switch (scanDimension)
		{
			case RScanDim::X: 
			{
				//EnclaveKeyDef::EnclaveKey beginXKey(dimValue, scansBegin->twoDKeyA.a, scansBegin->twoDKeyA.b);
				//EnclaveKeyDef::EnclaveKey endXKey(dimValue, scansBegin->twoDKeyB.a, scansBegin->twoDKeyB.b);
				//startKey = beginXKey;
				//endKey = endXKey;
				selectedDim = BrasenhamMassTracerDim::X;
				break;
			};
			case RScanDim::Y:
			{
				//EnclaveKeyDef::EnclaveKey beginYKey(scansBegin->twoDKeyA.a, dimValue, scansBegin->twoDKeyA.b);
				//EnclaveKeyDef::EnclaveKey endYKey(scansBegin->twoDKeyB.a, dimValue, scansBegin->twoDKeyB.b);
				//startKey = beginYKey;
				//endKey = endYKey;
				selectedDim = BrasenhamMassTracerDim::Y;
				break;
			}
			case RScanDim::Z:
			{
				//EnclaveKeyDef::EnclaveKey beginZKey(scansBegin->twoDKeyA.a, scansBegin->twoDKeyA.b, dimValue);
				//EnclaveKeyDef::EnclaveKey endZKey(scansBegin->twoDKeyB.a, scansBegin->twoDKeyB.b, dimValue);
				//startKey = beginZKey;
				//endKey = endZKey;
				selectedDim = BrasenhamMassTracerDim::Z;
				break;
			}
		}

		/*
		float pointA_x = (startKey.x * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		float pointA_y = (startKey.y * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		float pointA_z = (startKey.z * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		glm::vec3 scanRunPointA(pointA_x, pointA_y, pointA_z);

		float pointB_x = (endKey.x * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		float pointB_y = (endKey.y * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		float pointB_z = (endKey.z * in_rPolyRCubeDimLength) + (in_rPolyRCubeDimLength / 2);
		glm::vec3 scanRunPointB(pointB_x, pointB_y, pointB_z);

		interiorTracer.setData(startKey, 
							   endKey, 
			                   scanRunPointA, 
			                   scanRunPointB, 
			                   in_rPolyRCubeDimLength, 
			                   in_rPolyTileWeightToHundredthFloatRatio, 
			                   false);
		interiorTracer.setDownfillCrustBit(in_downfillCrustBitValue);
		interiorTracer.setUpfillCrustBit(in_upfillCrustBitValue);
		interiorTracer.setOptionalMaterialID(in_scanRunMaterialID);
		interiorTracer.setEmptyNormal(in_emptyNormal);
		interiorTracer.runTrace();
		*/

		BrasenhamMassTracer tracer(in_massGridArrayRef,
									scansBegin->twoDKeyA,
									scansBegin->twoDKeyB,
									selectedDim,
									in_rPolyTilesPerDim,
									dimValue,
									in_scanRunMaterialID);
		tracer.setDownfillCrustBit(in_downfillCrustBitValue);
		tracer.setUpfillCrustBit(in_upfillCrustBitValue);
		tracer.executeRun();



		//registerTotalIntetiorFills += interiorTracer.getTotalInserts();
		totalScansRan++;
	}
	
	//std::cout << "!! total scans ran: " << totalScansRan << std::endl;
}