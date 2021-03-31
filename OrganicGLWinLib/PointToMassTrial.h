#pragma once

#ifndef POINTTOMASSTRIAL_H
#define POINTTOMASSTRIAL_H

#include <map>
#include <glm/glm.hpp>
#include "MappableJudge.h"
#include "CollectiveVerdictMachine.h"


class PointToMassTrial
{
	public:
		void insertShellSlice(glm::vec3 in_pointToJudge, 
							int in_sPolyID,
							int in_sTriangleID,
							STriangle* in_sTriangleRef,
							glm::vec3 in_shellSliceBaseEmptyNormal,
							std::map<int, SPoly*> in_shellSliceClippingShellMap
							);
		void printJudgeMetaData();
		void executeAllJudgements();
		std::vector<glm::vec3> clippablePointsVector;	// contains any points that should be clipped from PointToSPolyRelationshipTrackerContainer
	private:
		std::map<int, MappableJudge> judgeMap;
		CollectiveVerdictMachine collectiveVerdicts;
};

#endif