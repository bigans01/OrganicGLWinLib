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

	//while (currentCubeKey != endCubeKey)
	while 
	( 
		(remainingDistance > 0.0f) 
		&&
		(currentCubeKey != endCubeKey)
	)
	{
		iterateToNextBlock();

		if (debugFlag == true)
		{
			std::cout << "!!! :::: remaining distance to travel: " << remainingDistance << std::endl;
			int debugWait = 3;
			//std::cin >> debugWait;
		}
	}

	std::cout << ":::::::::::: Final value of current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
}