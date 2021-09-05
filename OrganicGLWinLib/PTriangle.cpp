#include "stdafx.h"
#include "PTriangle.h"

PTriangle::PTriangle(RCollisionPoint* in_candidatePoint0Ref, 
					 RCollisionPoint* in_candidatePoint1Ref, 
					 RCollisionPoint* in_candidatePoint2Ref, 
					 glm::vec3 in_targetEmptyNormal, 
	                 bool in_pTriangleDebugFlag)
{
	currentEmptyNormal = in_targetEmptyNormal;	// the initial value of the normal calculated by all 3 points must equal this, when the resulting triangle's normal is normalized.
												// if it isn't, points 1 and 2 of the triangle must be swapped.
	collisionPointRefArray[0] = in_candidatePoint0Ref;
	collisionPointRefArray[1] = in_candidatePoint1Ref;
	collisionPointRefArray[2] = in_candidatePoint2Ref;

	//std::cout << "target empty normal is: " << currentEmptyNormal.x << ", " << currentEmptyNormal.y << ", " << currentEmptyNormal.z << std::endl;

	bool didTestPass = runInitialEmptyNormalTest(in_pTriangleDebugFlag);
	if (didTestPass == false)
	{
		// the test failed, so the order of points must be changed to produce the correct value of the empty normal.
		RCollisionPoint* indexOneCopy = collisionPointRefArray[1];
		collisionPointRefArray[1] = collisionPointRefArray[2];
		collisionPointRefArray[2] = indexOneCopy;

		runInitialEmptyNormalTest(in_pTriangleDebugFlag);
	}
}

PTrianglePointLinkArray PTriangle::fetchLinkArray()
{
	PTrianglePointLinkArray returnLinkArray;
	for (int x = 0; x < 3; x++)
	{
		PTriangleRCPointToIndexLink currentLink(collisionPointRefArray[x], x);
		returnLinkArray.linkArray[x] = currentLink;
	}
	return returnLinkArray;
}

void PTriangle::rebuildEmptyNormal()
{
	glm::vec3 collisionPoint0CurrentValue = collisionPointRefArray[0]->currentValue;
	glm::vec3 collisionPoint1CurrentValue = collisionPointRefArray[1]->currentValue;
	glm::vec3 collisionPoint2CurrentValue = collisionPointRefArray[2]->currentValue;

	glm::vec3 u = collisionPoint1CurrentValue - collisionPoint0CurrentValue;
	glm::vec3 v = collisionPoint2CurrentValue - collisionPoint0CurrentValue;

	currentEmptyNormal = cross(u, v);
}

glm::vec3 PTriangle::getEmptyNormal()
{
	return currentEmptyNormal;
}

void PTriangle::printPTrianglePoints()
{
	std::cout << "=== Printing PTriangle points: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		std::cout << x << ": " << collisionPointRefArray[x]->currentValue.x << ", " << collisionPointRefArray[x]->currentValue.y << ", " << collisionPointRefArray[x]->currentValue.z << std::endl;
	}
}

short PTriangle::getMaterialID()
{
	return pTriangleMaterialID;
}

void PTriangle::calculateTriangleMaterial()
{
	std::map<short, int> materialCountMap;
	for (int x = 0; x < 3; x++)
	{
		materialCountMap[collisionPointRefArray[x]->sampledMaterialID]++;
	}

	if (materialCountMap.size() == 3)
	{
		auto firstMaterial = materialCountMap.begin();
		pTriangleMaterialID = firstMaterial->first;
	}
	else if (materialCountMap.size() == 2)
	{
		auto materialFinderBegin = materialCountMap.begin();
		auto materialFinderEnd = materialCountMap.end();
		short targetMaterialValue = 0;
		for (; materialFinderBegin != materialFinderEnd; materialFinderBegin++)
		{
			if (materialFinderBegin->second == 2)
			{
				targetMaterialValue = materialFinderBegin->first;
				break;
			}
		}

		pTriangleMaterialID = targetMaterialValue;
	}
	else if (materialCountMap.size() == 1)
	{
		auto firstMaterial = materialCountMap.begin();
		pTriangleMaterialID = firstMaterial->first;
	}
}

void PTriangle::sampleTriangleMaterial(MassGridArray* in_massGridArrayRef,
	float in_tileGridWidth,
	int in_numberOfTilesPerDimension)
{
	EnclaveKeyDef::EnclaveKey targetSamplingGridCell = RUtils::findRasterGridCellCentroid(collisionPointRefArray[0]->currentValue,
		collisionPointRefArray[1]->currentValue,
		collisionPointRefArray[2]->currentValue,
		in_tileGridWidth, in_numberOfTilesPerDimension);

	auto searchResult = in_massGridArrayRef->searchForCell(targetSamplingGridCell.x, targetSamplingGridCell.y, targetSamplingGridCell.z);
	if (searchResult.wasSearchKeyValid == true)
	{
		//std::cout << "(PTriangle): found cell; number of flags in cell is: " << searchResult.cellRef->getNumberOfFlagsSet() << std::endl;
		pTriangleMaterialID = searchResult.cellRef->getCellMaterialID();
		//pTriangleMaterialID = 2;
		if (searchResult.cellRef->getNumberOfFlagsSet() != 0)
		{
			/*
			std::cout << "!!! Notice, cell at (" << targetSamplingGridCell.x << ", " << targetSamplingGridCell.y << ", " << targetSamplingGridCell.z << ") had " << searchResult.cellRef->getNumberOfFlagsSet() << " flags set. " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "!!! Point " << x << " currentValue: " << collisionPointRefArray[x]->currentValue.x << ", " << collisionPointRefArray[x]->currentValue.y << ", " << collisionPointRefArray[x]->currentValue.z << std::endl;
			}

			*/
		}


	}
	else if (searchResult.wasSearchKeyValid == false)
	{
		//std::cout << "(PTriangle): normal sampling; invalid sample found. " << std::endl;
	}
}

bool PTriangle::runInitialEmptyNormalTest(bool in_pTriangleDebugFlag)
{
	bool wasValid = true;

	glm::vec3 currentCollisionPoint0Point = collisionPointRefArray[0]->originalValue;
	glm::vec3 currentCollisionPoint1Point = collisionPointRefArray[1]->originalValue;
	glm::vec3 currentCollisionPoint2Point = collisionPointRefArray[2]->originalValue;

	glm::vec3 u = currentCollisionPoint1Point - currentCollisionPoint0Point;
	glm::vec3 v = currentCollisionPoint2Point - currentCollisionPoint0Point;

	glm::vec3 testNormal = cross(u, v);
	ECBPolyPoint pointToNormalize(testNormal.x, testNormal.y, testNormal.z);

	ECBPolyPoint normalizedNormal = IndependentUtils::findNormalizedPoint(pointToNormalize);
	glm::vec3 normalizedNormalConverted(normalizedNormal.x, normalizedNormal.y, normalizedNormal.z);

	if (in_pTriangleDebugFlag == true)
	{
		std::cout << "normalized normal converted: " << normalizedNormalConverted.x << ", " << normalizedNormalConverted.y << ", " << normalizedNormalConverted.z << std::endl;
	}

	if
		(
		(normalizedNormalConverted == currentEmptyNormal)
			&&
			(in_pTriangleDebugFlag == true)
			)
	{
		std::cout << "!!! test Normal matches target current Empty normal, no point swap required! " << std::endl;
		std::cout << "Current empty normal is: " << currentEmptyNormal.x << ", " << currentEmptyNormal.y << ", " << currentEmptyNormal.z << std::endl;
	}
	else
	{
		wasValid = false;
		if (in_pTriangleDebugFlag == true)
		{
			std::cout << "!!! test Normal DOESN't match target; swapping. " << std::endl;
		}
	}

	return wasValid;
};
