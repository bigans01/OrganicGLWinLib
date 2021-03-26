#pragma once

#ifndef MASSZONESHELLSLICE_H
#define MASSZONESHELLSLICE_H

#include "STriangle.h"
#include <glm/glm.hpp>
#include <map>
#include "SPoly.h"
#include "PointToMassRelationshipType.h"
#include "QuatUtils.h"

class MassZoneShellSlice
{
	public:
		MassZoneShellSlice(STriangle* in_sTriangleRef,
							glm::vec3 in_shellSliceBaseEmptyNormal,
							glm::vec3 in_relationshipPointToCompare,
							std::map<int, SPoly*> in_shellSliceClippingShellMap) :
			shellSliceBaseTriangle(in_sTriangleRef),
			shellSliceBaseEmptyNormal(in_shellSliceBaseEmptyNormal),
			relationshipPointToCompare(in_relationshipPointToCompare),
			shellSliceClippingShellMap(in_shellSliceClippingShellMap)
		{
			runAnalysis();
		};
	private:
		STriangle* shellSliceBaseTriangle = nullptr;		// the STriangle representing the base of this slice
		glm::vec3 shellSliceBaseEmptyNormal;				// empty normal used by the base STriangle; used to calculate appropriate ray
		glm::vec3 relationshipPointToCompare;				// the point that we will be comparing against the MassZoneShellSlice
		std::map<int, SPoly*> shellSliceClippingShellMap;	// the clipping shell map, containing all other SPolys of the MassZone's shell that we must compare this slice with
		PointToMassRelationshipType analysisResult = PointToMassRelationshipType::NOVAL;	// stores the analysis result of comparing the relationshipPointToCompare to this shell slice (i.e, WITHIN_MASS, 
		void runAnalysis();
};

#endif

