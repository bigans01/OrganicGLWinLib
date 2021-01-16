#pragma once

#ifndef FUSEDPOINTCONTAINER_H
#define FUSEDPOINTCONTAINER_H

#include <map>
#include "FusedPoint.h"

class FusedPointContainer
{
	public:
		void insertSubDataForPoint(glm::vec3 in_point, FusedPointSubData in_fusedPointSubData);
		void clearFusedPoints();
		void printFusedPoints();
	private:
		std::map<int, FusedPoint> fusedPointMap;

};

#endif
