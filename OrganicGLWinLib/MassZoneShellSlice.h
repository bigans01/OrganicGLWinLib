#pragma once

#ifndef MASSZONESHELLSLICE_H
#define MASSZONESHELLSLICE_H

#include "STriangle.h"
#include <glm/glm.hpp>
#include <map>
#include "SPoly.h"
#include "PointToMassRelationshipType.h"
#include "QuatUtils.h"
#include "Ray.h"
#include "RaycastIntersectionTest.h"
#include "RayIntersectionResult.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class MassZoneShellSlice
{
	public:
		MassZoneShellSlice() {};
	private:
		friend class PointToMassRelationshipJudge;
		friend class PointToMassTrial;
		MassZoneShellSlice(STriangle* in_sTriangleRef,
							int in_baseSPolyID,
						    int in_baseSTriangleID,
							glm::vec3 in_shellSliceBaseEmptyNormal,
							glm::vec3 in_relationshipPointToCompare,
							std::map<int, SPoly*> in_shellSliceClippingShellMap) :
			shellSliceBaseTriangle(in_sTriangleRef),
			baseSPolyID(in_baseSPolyID),
			baseSTriangleID(in_baseSTriangleID),
			shellSliceBaseEmptyNormal(in_shellSliceBaseEmptyNormal),
			relationshipPointToCompare(in_relationshipPointToCompare),
			shellSliceClippingShellMap(in_shellSliceClippingShellMap)
		{};
		PointToMassRelationshipType getAnalysisResult();
		STriangle* shellSliceBaseTriangle = nullptr;		// the STriangle representing the base of this slice
		glm::vec3 shellSliceBaseEmptyNormal;				// empty normal used by the base STriangle; used to calculate appropriate ray
		glm::vec3 relationshipPointToCompare;				// the point that we will be comparing against the MassZoneShellSlice
		std::map<int, SPoly*> shellSliceClippingShellMap;	// the clipping shell map, containing all other SPolys of the MassZone's shell that we must compare this slice with
		PointToMassRelationshipType analysisResult = PointToMassRelationshipType::NOVAL;	// stores the analysis result of comparing the relationshipPointToCompare to this shell slice (i.e, WITHIN_MASS, 
		PolyLogger shellSlicePolyLogger;
		PolyDebugLevel shellSlicePolyLoggerDebugLevel = PolyDebugLevel::NONE;
		int baseSPolyID = 0;		// the SPoly this shell slice's base is spawned from
		int baseSTriangleID = 0;	// the exact STriangle in the SPoly that this shell slice's base is spawned from

		void setShellSliceDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void runAnalysis();	// should only need to run once
		void checkLineOfSightForWithinMassRelationship();

};

#endif

