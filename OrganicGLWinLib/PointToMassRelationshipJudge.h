#pragma once

#ifndef POINTTOMASSRELATIONSHIPJUDGE_H
#define POINTTOMASSRELATIONSHIPJUDGE_H

#include <map>
#include "MassZoneShellSlice.h"

class PointToMassRelationshipJudge
{
	public:
		void insertShellSliceForSPolyID(int in_sPolyID,
			STriangle* in_sTriangleRef,
			glm::vec3 in_shellSliceBaseEmptyNormal,
			glm::vec3 in_relationshipPointToCompare,
			std::map<int, SPoly*> in_shellSliceClippingShellMap);
	private:
		friend class PointToMassTrial;
		friend class MappableJudge;
		void executeJudgementOnShellSlices();
		std::map<int, MassZoneShellSlice> shellSliceMap;	// the key value (int) here should be equal to the value of the SPoly.
};

#endif
