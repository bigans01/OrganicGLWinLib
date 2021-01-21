#pragma once

#ifndef FUSEDPOINTCONTAINER_H
#define FUSEDPOINTCONTAINER_H

#include <map>
#include "FusedPoint.h"
#include "FusedPointMeta.h"
#include "FusionCandidateOrigin.h"
#include "FusedPointSubData.h"
#include <vector>

class FusedPointContainer
{
	public:
		friend class FusedPointReactor;
		friend class GuestLineReactor;
		void insertSubDataForPoint(glm::vec3 in_point, FusedPointSubData in_fusedPointSubData);
		void clearFusedPoints();
		void printFusedPoints();
		void loadPointsIntoOtherContainer(FusedPointContainer* in_otherFusedPointContainer);
		FusedPointMeta retrieveFusedPointMeta(glm::vec3 in_pointToRetrieve, FusionCandidateOrigin in_originToMarkAs);
		FusedPointMeta retrieveOtherFusedPointMeta(glm::vec3 in_pointToRetrieve, FusionCandidateOrigin in_originToMarkAs);
		std::vector<FusedPointSubData>* fetchSubDataVectorForPoint(glm::vec3 in_pointToRetrieveFor);
	private:
		std::map<int, FusedPoint> fusedPointMap;

};

#endif
