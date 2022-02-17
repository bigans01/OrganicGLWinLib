#include "stdafx.h"
#include "RCollisionPointCaster.h"

void RCollisionPointCaster::runTrace()
{
	//std::cout << ">>>> attempting RCollisionPointCaster trace..." << std::endl;
	optionalLoggerRef->log("(RCollisionPointCaster): >>>> attempting RCollisionPointCaster trace...", "\n");
	bool casterContinuationFlag = true;

	// check if the very first initial block has mass; don't bother continuing if it does
	MassGridSearchResult initialBlockSearchResult = casterMassGridArrayRef->searchForCell(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z);
	if (initialBlockSearchResult.wasSearchKeyValid == true)
	{
		if (initialBlockSearchResult.cellRef->getNumberOfFlagsSet() > 0)
		{
			//std::cout << "### Notice: very first currentCubeKey value has mass; discarding while loop. " << std::endl;
			optionalLoggerRef->log("(RCollisionPointCaster): ### Notice: very first currentCubeKey value has mass; discarding while loop, ", "\n");
			// even if found initially as being with mass, update it as being moved.


			casterContinuationFlag = false;
			glm::vec3 convertedDestinationPointVec3 = RUtils::convertGridCellLocationToVec3(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z, rasterCubeDimLength);
			pointToMoveRef->currentValue = convertedDestinationPointVec3;
			pointToMoveRef->setCollisionPointState(RCollisionPointState::MOVED);
			pointToMoveRef->setCollisionPointMaterial(initialBlockSearchResult.cellRef->getCellMaterialID());
		}
		else
		{
			// since we're continuing, go ahead and set the TRACE_BIT; we don't want to set this bit if the casterContinuationFlag is 
			// set to false, because of the very first initialBlockSearchResult's metadata.
			initialBlockSearchResult.cellRef->setFlag(MassCellBitFlags::TRACE_BIT, 1);
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
				//std::cout << "!! Notice, point caster attempt found flags at block having key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
				optionalLoggerRef->log("(RCollisionPointCaster): !! Notice, point caster attempt found flags at block having key: ", currentCubeKey.x, ", ", currentCubeKey.y, ", ", currentCubeKey.z, "\n");


				// if found, update the point as being moved.

				casterContinuationFlag = false;
				glm::vec3 convertedDestinationPointVec3 = RUtils::convertGridCellLocationToVec3(currentCubeKey.x, currentCubeKey.y, currentCubeKey.z, rasterCubeDimLength);
				pointToMoveRef->currentValue = convertedDestinationPointVec3;
				pointToMoveRef->setCollisionPointState(RCollisionPointState::MOVED);
				pointToMoveRef->setCollisionPointMaterial(currentBlockSearchResult.cellRef->getCellMaterialID());


			}
		
			// set the TRACE_BIT on all iterations of this while loop; will also set the TRACE_BIT in the very last run where casterContinuationFlag gets set to false.
			currentBlockSearchResult.cellRef->setFlag(MassCellBitFlags::TRACE_BIT, 1);
			
		}
	}

	//std::cout << ">>>> finished RCollisionPointCaster trace..." << std::endl;
	//std::cout << ":::::::::::: Final value of current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;

	optionalLoggerRef->log("(RCollisionPointCaster): >>>> finished RCollisionPointCaster trace...", "\n");
	optionalLoggerRef->log("(RCollisionPointCaster): :::::::::::: Final value of current cube key: ", currentCubeKey.x, ", ", currentCubeKey.y, ", ", currentCubeKey.z, "\n");
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