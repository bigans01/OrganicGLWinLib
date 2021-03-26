#pragma once

#ifndef POINTTOMASSTRIAL_H
#define POINTTOMASSTRIAL_H

#include <map>
#include <glm/glm.hpp>
#include "MappableJudge.h"


class PointToMassTrial
{
	public:
		void insertShellSlice(glm::vec3 in_pointToJudge, int in_sPolyID,
			STriangle* in_sTriangleRef,
			glm::vec3 in_shellSliceBaseEmptyNormal,
			std::map<int, SPoly*> in_shellSliceClippingShellMap);
		void printJudgeMetaData();
		void executeAllJudgements();
	private:
		std::map<int, MappableJudge> judgeMap;
};

#endif