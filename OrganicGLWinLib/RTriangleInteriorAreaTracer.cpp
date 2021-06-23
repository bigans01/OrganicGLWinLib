#include "stdafx.h"
#include "RTriangleInteriorAreaTracer.h"

void RTriangleInteriorAreaTracer::setGridArrayRef(MassGridArray* in_massGridArrayRef)
{
	gridArrayRef = in_massGridArrayRef;
}

void RTriangleInteriorAreaTracer::setDownfillCrustBit(int in_downfillCrustBitValue)
{
	downfillCrustBitValue = in_downfillCrustBitValue;
}

void RTriangleInteriorAreaTracer::runTrace()
{
	//std::cout << ">>> begin trace run: start key (" << startCubeKey.x << ", " << startCubeKey.y << "," << startCubeKey.z 
	//	                       << ") | end key: (" << endCubeKey.x << ", " << endCubeKey.y << ", " << endCubeKey.z << std::endl;
	auto traceAttemptBegin = std::chrono::high_resolution_clock::now();
	int numberOfInserts = 0;
	/*
	MassGridSearchResult result = gridArrayRef->searchForCell(startCubeKey.x, startCubeKey.y, startCubeKey.z);
	if (result.wasSearchKeyValid == true)
	{
		result.cellRef->setFlag(MassCellBitFlags::CRUST_MASS, 1);
		result.cellRef->setFlag(MassCellBitFlags::INNER_MASS, 1);
		numberOfInserts = 1;
	}
	*/
	while
	(
		(remainingDistance > 0.0f)
		&&
		(currentCubeKey != endCubeKey)
	)
	{
		iterateToNextBlock();
		MassGridSearchResult result = gridArrayRef->searchForCell(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z);
		if (result.wasSearchKeyValid == true)
		{
			result.cellRef->setFlag(MassCellBitFlags::CRUST_MASS, 1);
			result.cellRef->setFlag(MassCellBitFlags::INNER_MASS, 1);
			result.cellRef->setFlag(MassCellBitFlags::DOWNFILL_CRUST, downfillCrustBitValue);
		}
		numberOfInserts++;

		if (debugFlag == true)
		{
			std::cout << "!!! :::: remaining distance to travel: " << remainingDistance << std::endl;
			int debugWait = 3;
			//std::cin >> debugWait;
		}

		// if the remaining distance goes negative, BEFORE the endCubeKey is hit, we've gone too far, and must attempt another pass.
		if
		(
			(remainingDistance < 0.0f)
			&&
			(currentCubeKey != endCubeKey)
		)
		{
			EnclaveKeyDef::EnclaveKey nextRunPointAKey = currentCubeKey;
			EnclaveKeyDef::EnclaveKey nextRunPointBKey = endCubeKey;
			glm::vec3 nextRunCubeAPoint(
				(float(currentCubeKey.x) + 0.5f) * tileWeightRatio,
				(float(currentCubeKey.y) + 0.5f) * tileWeightRatio,
				(float(currentCubeKey.z) + 0.5f) * tileWeightRatio
			);
			glm::vec3 nextRunCubeBPoint(
				(float(endCubeKey.x) + 0.5f) * tileWeightRatio,
				(float(endCubeKey.y) + 0.5f) * tileWeightRatio,
				(float(endCubeKey.z) + 0.5f) * tileWeightRatio
			);

			setUpNextRun(nextRunPointAKey, nextRunPointBKey, nextRunCubeAPoint, nextRunCubeBPoint);
		}
	}




	auto traceAttemptEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> comparisonIterationsElapsed = traceAttemptEnd - traceAttemptBegin;
	//std::cout << "#-> Raster trace time  > " << comparisonIterationsElapsed.count() << std::endl;
	//std::cout << "#-> Number of inserts: " << numberOfInserts << std::endl;
	//std::cout << "#-> Remaining distance to travel after run is done: " << remainingDistance << std::endl;

	//std::cout << ":::::::::::: Final value of current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
	if (currentCubeKey == endCubeKey)
	{
		isRunComplete = true;
	}
}