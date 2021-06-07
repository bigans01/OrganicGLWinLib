#include "stdafx.h"
#include "RTriangleLineTracer.h"

void RTriangleLineTracer::runTrace()
{
	std::cout << "======== stats for RTriangleLineTracer, prior to call of iterate to next block: " << std::endl;
	std::cout << "Point A Cube Key: " << startCubeKey.x << ", " << startCubeKey.y << ", " << startCubeKey.z << std::endl;
	std::cout << "Point B Cube Key: " << endCubeKey.x << ", " << endCubeKey.y << ", " << endCubeKey.z << std::endl;
	std::cout << "Start cube point: " << startCubePoint.x << ", " << startCubePoint.y << ", " << startCubePoint.z << std::endl;
	std::cout << "End cube point: " << endCubePoint.x << ", " << endCubePoint.y << ", " << endCubePoint.z << std::endl;
	std::cout << "Tracing vector: " << tracerDirectionVector.x << ", " << tracerDirectionVector.y << ", " << tracerDirectionVector.z << std::endl;
	std::cout << "Normalized slope: " << normalizedSlopeDirection.x << ", " << normalizedSlopeDirection.y << ", " << normalizedSlopeDirection.z << std::endl;

	std::cout << "Line length: " << lineLength << std::endl;

	auto traceAttemptBegin = std::chrono::high_resolution_clock::now();
	optionalCubeLookup->insertCubeKey(currentCubeKey);
	int numberOfInserts = 1;

	while 
	( 
		(remainingDistance > 0.0f) 
		&&
		(currentCubeKey != endCubeKey)
	)
	{
		iterateToNextBlock();
		optionalCubeLookup->insertCubeKey(currentCubeKey);
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
	std::cout << "#-> Raster trace time  > " << comparisonIterationsElapsed.count() << std::endl;
	std::cout << "#-> Number of inserts: " << numberOfInserts << std::endl;

	std::cout << ":::::::::::: Final value of current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
	if (currentCubeKey == endCubeKey)
	{
		isRunComplete = true;
	}
}