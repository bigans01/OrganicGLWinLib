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
		void createArray(int in_arraySize);
		RCollisionPointSearchResult doesRCollisionPointExist(glm::vec3 in_pointToSearch);
		void insertRCollisionPoint(ECBPPOrientations in_orientation, glm::vec3 in_pointToFind);

	private:
		bool isArraySet = false;
		std::unique_ptr<RCollisionPoint[]> RCollisionPoints;
		int arraySize = 0;
		int currentArrayIndex = 0;
};

#endif
