#pragma once

#ifndef COLLISIONPOINTARRAY_H
#define COLLISIONPOINTARRAY_H

#include <mutex>
#include "CollisionPoint.h"
#include "CollisionPointSearchResult.h"
#include "ECBPPOrientations.h"

class CollisionPointArray
{
	public:
		CollisionPointArray() {};
		void createArray(int in_arraySize);
		CollisionPointSearchResult doesCollisionPointExist(glm::vec3 in_pointToSearch);
		void insertCollisionPoint(ECBPPOrientations in_orientation, glm::vec3 in_pointToFind);

	private:
		bool isArraySet = false;
		std::unique_ptr<CollisionPoint[]> collisionPoints;
		int arraySize = 0;
		int currentArrayIndex = 0;
};

#endif
