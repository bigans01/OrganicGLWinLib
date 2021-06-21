#pragma once

#ifndef RCollisionPointARRAY_H
#define RCollisionPointARRAY_H

#include <mutex>
#include "RCollisionPoint.h"
#include "RCollisionPointSearchResult.h"
#include "ECBPPOrientations.h"

class RCollisionPointArray
{
	public:
		RCollisionPointArray() {};
		RCollisionPointArray& operator=(const RCollisionPointArray& in_arrayB)
		{
			isArraySet = in_arrayB.isArraySet;
			arraySize = in_arrayB.arraySize;
			currentArrayIndex = in_arrayB.currentArrayIndex;
			if (isArraySet == true)
			{
				collisionPoints.reset(new RCollisionPoint[arraySize]);
				for (int x = 0; x < arraySize; x++)
				{
					collisionPoints[x] = in_arrayB.collisionPoints[x];
				}
			}
			return *this;
		};

		void createArray(int in_arraySize);
		RCollisionPointSearchResult doesRCollisionPointExist(glm::vec3 in_pointToSearch);
		void insertRCollisionPoint(ECBPPOrientations in_orientation, glm::vec3 in_pointToFind);

	private:
		bool isArraySet = false;
		std::unique_ptr<RCollisionPoint[]> collisionPoints;
		int arraySize = 0;
		int currentArrayIndex = 0;
};

#endif