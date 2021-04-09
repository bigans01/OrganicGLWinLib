#pragma once

#ifndef POINTTOMASSRELATIONSHIPJUDGE_H
#define POINTTOMASSRELATIONSHIPJUDGE_H

#include <map>
#include "MassZoneShellSlice.h"
#include "IndividualVerdict.h"

class PointToMassRelationshipJudge
{
	public:
		void insertShellSliceForSPolyID(int in_sPolyID,
			int in_sTriangleID,
			STriangle* in_sTriangleRef,
			glm::vec3 in_shellSliceBaseEmptyNormal,
			glm::vec3 in_relationshipPointToCompare,
			std::map<int, SPoly*> in_shellSliceClippingShellMap);
	private:
		friend class PointToMassTrial;
		friend class MappableJudge;
		IndividualVerdict executeJudgementOnShellSlices();			// the bool value returned indicates whether or not the point should be clipped from the PointToSPolyRelationshipTracker;
														// if the bool is true, it should be clipped.
		std::map<int, MassZoneShellSlice> shellSliceMap;	// the key value (int) here should be equal to the value of the SPoly.
		std::map<int, PointToMassRelationshipType> analysisMap;	// contains analysis results from the judgements run on each shell slice.
		glm::vec3 pointToCompareSlicesAgainst;			// the point compared against all shell slices; set during any call to insertShellSliceForSPolyID.

		bool determineVerdict();
		bool runTestDoAllHaveNoLineOfSight();
		bool runTestAnyCasesOfWithinOrCoplanar();
		bool runTestAnyCaseOfOutsideOfMass();

		struct JudgeTriangleLocation
		{
			JudgeTriangleLocation() {};
			JudgeTriangleLocation(int in_sPolyID, int in_sTriangleID) :
				sPolyID(in_sPolyID),
				sTriangleID(in_sTriangleID)
			{};
			public:
				int sPolyID = 0;
				int sTriangleID = 0;
		};

		std::map<int, JudgeTriangleLocation> locationMap;
};

#endif
