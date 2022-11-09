#pragma once

#ifndef FUSEDPOINTCONTAINER_H
#define FUSEDPOINTCONTAINER_H

#include "FusedPoint.h"
#include "FusedPointMeta.h"
#include "FusionCandidateOrigin.h"
#include "FusedPointSubData.h"

class FusedPointContainer
{
	public:
		friend class FusedPointReactor;
		friend class GuestLineReactor;
		void insertSubDataForPoint(glm::vec3 in_point, FusedPointSubData in_fusedPointSubData);
		void rebuildSubDataForPointForTwoBorderLines(glm::vec3 in_point, std::map<int, int> in_lineMap);	// this function is used by HostLineReactor::buildASliceSingleInterceptsPointPrecise,
																											// in order to guarantee that the std::vector<FusedPointSubData> used in that function contains exactly two border lines.
																											// It was observed on or around 8/3/2022, that the previous code in that function doesn't guarantee that there are two border lines in the
																											// vector, which must always be the case for IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE.
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
