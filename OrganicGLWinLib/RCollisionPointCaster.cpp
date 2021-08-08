#include "stdafx.h"
#include "RCollisionPointCaster.h"

void RCollisionPointCaster::runTrace()
{
	std::cout << ">>>> attempting RCollisionPointCaster trace..." << std::endl;
	bool casterContinuationFlag = true;

	// check if the very first initial block has mass; don't bother continuing if it does
	MassGridSearchResult initialBlockSearchResult = casterMassGridArrayRef->searchForCell(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z);
	if (initialBlockSearchResult.wasSearchKeyValid == true)
	{
		if (initialBlockSearchResult.cellRef->getNumberOfFlagsSet() > 0)
		{
			std::cout << "### Notice: very first currentCubeKey value has mass; discarding while loop. " << std::endl;

			// even if found initially as being with mass, update it as being moved.


			casterContinuationFlag = false;
			glm::vec3 convertedDestinationPointVec3 = RUtils::convertGridCellLocationToVec3(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z, rasterCubeDimLength);
			pointToMoveRef->currentValue = convertedDestinationPointVec3;
			pointToMoveRef->setCollisionPointState(RCollisionPointState::MOVED);
		}
	}

	while
	(
		(remainingDistance > 0.0f)
		&&
		(currentCubeKey != endCubeKey)
		&&
		(casterContinuationFlag == true)
	)
	{
		iterateToNextBlock();
		MassGridSearchResult currentBlockSearchResult = casterMassGridArrayRef->searchForCell(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z);
		if (currentBlockSearchResult.wasSearchKeyValid == true)
		{
			if (currentBlockSearchResult.cellRef->getNumberOfFlagsSet() > 0)
			{
				std::cout << "!! Notice, point caster attempt found flags at block having key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;

				// if found, update the point as being moved.

				casterContinuationFlag = false;
				glm::vec3 convertedDestinationPointVec3 = RUtils::convertGridCellLocationToVec3(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z, rasterCubeDimLength);
				pointToMoveRef->currentValue = convertedDestinationPointVec3;
				pointToMoveRef->setCollisionPointState(RCollisionPointState::MOVED);
			}
		}
	}

	std::cout << ">>>> finished RCollisionPointCaster trace..." << std::endl;
	std::cout << ":::::::::::: Final value of current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
}

void RCollisionPointCaster::setGridArrayRef(MassGridArray* in_optionalMassGridArrayRef)
{
	casterMassGridArrayRef = in_optionalMassGridArrayRef;
}

void RCollisionPointCaster::setPointToMoveRef(RCollisionPoint* in_destinationPointRef)
{
	pointToMoveRef = in_destinationPointRef;
}

glm::vec3 RCollisionPointCaster::getConvertedTraceEndPoint()
{
	return RUtils::convertGridCellLocationToVec3(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z, rasterCubeDimLength);
}