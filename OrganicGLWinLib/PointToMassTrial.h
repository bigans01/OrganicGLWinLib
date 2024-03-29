#pragma once

#ifndef POINTTOMASSTRIAL_H
#define POINTTOMASSTRIAL_H

#include "MappableJudge.h"
#include "CollectiveVerdictMachine.h"
#include "PolyDebugLevel.h"


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
		bool executeAllJudgements(PolyDebugLevel in_polyDebugLevel);	// run judgements; check if the SPoly is a "nacho dip" SPoly.
		std::vector<glm::vec3> clippablePointsVector;	// contains any points that should be clipped from PointToSPolyRelationshipTrackerContainer
	private:
		std::map<int, MappableJudge> judgeMap;
		CollectiveVerdictMachine collectiveVerdicts;
};

#endif