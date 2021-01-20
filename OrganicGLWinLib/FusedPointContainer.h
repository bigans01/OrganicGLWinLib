#pragma once

#ifndef FUSEDPOINTCONTAINER_H
#define FUSEDPOINTCONTAINER_H

#include <map>
#include "FusedPoint.h"
#include "FusedPointMeta.h"
#include "FusionCandidateOrigin.h"

class FusedPointContainer
{
	public:
		friend class FusedPointReactor;
		void insertSubDataForPoint(glm::vec3 in_point, FusedPointSubData in_fusedPointSubData);
		void clearFusedPoints();
		void printFusedPoints();
		void loadPointsIntoOtherContainer(FusedPointContainer* in_otherFusedPointContainer);
		FusedPointMeta retrieveFusedPointMeta(glm::vec3 in_pointToRetrieve, FusionCandidateOrigin in_originToMarkAs);
	private:
		std::map<int, FusedPoint> fusedPointMap;

};

#endif
