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
			casterContinuationFlag = false;
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
				casterContinuationFlag = false;
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