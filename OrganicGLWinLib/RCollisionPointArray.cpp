#include "stdafx.h"
#include "RCollisionPointArray.h"

void RCollisionPointArray::createArray(int in_arraySize)
{
	arraySize = in_arraySize;	// would be equal to the number of points per MorphableMesh x the the number of MorphableMeshes; ie., for an Enclave Collection
								// where the morphable meshes each contain 8 points, the value would be 4096
	collisionPoints.reset(new RCollisionPoint[arraySize]);
	isArraySet = true;
}

RCollisionPoint* RCollisionPointArray::attemptPointInsert(glm::vec3 in_pointToSearch, ECBPPOrientationResults in_orientationResults)
{
	RCollisionPoint* returnPoint = nullptr;
	RCollisionPointSearchResult searchResult = doesRCollisionPointExist(in_pointToSearch);
	if (searchResult.wasPointFound == true)
	{
		returnPoint = &collisionPoints[searchResult.arrayIndex];
	}
	else if (searchResult.wasPointFound == false)
	{
		int insertedIndex = insertRCollisionPoint(in_orientationResults.osubtype, in_pointToSearch);
		returnPoint = &collisionPoints[insertedIndex];
	}
	return returnPoint;
}

RCollisionPointSearchResult RCollisionPointArray::doesRCollisionPointExist(glm::vec3 in_pointToSearch)
{
	RCollisionPointSearchResult searchResult;
	for (int x = 0; x < currentArrayIndex; x++)
	{
		if (collisionPoints[x].originalValue == in_pointToSearch)
		{
			searchResult.wasPointFound = true;
			searchResult.arrayIndex = x;
			searchResult.point = in_pointToSearch;
			break;
		}
	}
	return searchResult;
}

int RCollisionPointArray::insertRCollisionPoint(ECBPPOrientations in_orientation, glm::vec3 in_pointToFind)
{
	RCollisionPoint newPoint(in_orientation, in_pointToFind);
	int returnIndex = currentArrayIndex;
	collisionPoints[currentArrayIndex++] = newPoint;
	return returnIndex;
}