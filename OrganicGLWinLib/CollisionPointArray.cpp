#include "stdafx.h"
#include "CollisionPointArray.h"

void CollisionPointArray::createArray(int in_arraySize)
{
	arraySize = in_arraySize;	// would be equal to the number of points per MorphableMesh x the the number of MorphableMeshes; ie., for an Enclave Collection
								// where the morphable meshes each contain 8 points, the value would be 4096
	collisionPoints.reset(new CollisionPoint[arraySize]);
	isArraySet = true;
}

CollisionPointSearchResult CollisionPointArray::doesCollisionPointExist(glm::vec3 in_pointToSearch)
{
	CollisionPointSearchResult searchResult;
	for (int x = 0; x < currentArrayIndex; x++)
	{
		if (collisionPoints[x].originalValue == in_pointToSearch)
		{
			searchResult.wasPointFound = true;
			searchResult.point = in_pointToSearch;
			break;
		}
	}
	return searchResult;
}

void CollisionPointArray::insertCollisionPoint(ECBPPOrientations in_orientation, glm::vec3 in_pointToFind)
{
	CollisionPoint newPoint(in_orientation, in_pointToFind);
	collisionPoints[currentArrayIndex++] = newPoint;
}