#pragma once

#ifndef FUSEDPOINT_H
#define FUSEDPOINT_H

#include <vector>
#include "FusedPointSubData.h"
#include <glm/glm.hpp>
#include <iostream>
#include "FusedPointMeta.h"

class FusedPoint
{
	friend class FusedPointContainer;
	public:
		glm::vec3 point;
		bool doesPointMatch(glm::vec3 in_pointToMatchAgainst);
		void insertFusedPointSubData(FusedPointSubData in_fusedPointSubData);
		void printData();
		FusedPointMeta getMetaForPoint(glm::vec3 in_pointToGetMetaFor);
		std::vector<FusedPointSubData>* getSubDataVectorRef();
	private:
		std::vector<FusedPointSubData> subDataVector;
};

#endif
