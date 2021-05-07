#pragma once

#ifndef MASSZONEBOXBOUNDARYSPOLYSET_H
#define MASSZONEBOXBOUNDARYSPOLYSET_H

#include "SPoly.h"
#include "STriangle.h"
#include "IntersectionLineGroup.h"
#include "IntersectionResult.h"
#include "IntersectionLine.h"
#include "CategorizedLineColinearTester.h"
#include "PolyLogger.h"
#include "CoplanarChecker.h"
#include "PolyDebugLevel.h"
#include "FusedIntersectionLine.h"
#include "FusionCandidateProducer.h"
#include "FusionCandidateOrigin.h"
#include "FusedPointReactor.h"
#include "SPolyFracturer.h"
#include "SPolyMorphTracker.h"


class MassZoneBoxBoundarySPolySet
{
	public:
		friend class MassZone;
		friend class MassZoneBoxBoundary;
		friend class MassZoneBox;
		void setBoundarySPolyRef(SPoly* in_sPolyRef);
		void setLogLevel(PolyDebugLevel in_sPolyDebugLevel);
		void compareSPolySubZoneSPolyToBoundarySPoly(SPoly* in_sPolyRef);
		void insertCategorizedLinesFromNonboundarySPoly(SPoly* in_sPolyRef);
		void buildBoundarySPolyFromFactory();
		SPoly* boundarySPolyRef = nullptr;	// the SPoly in the boundary that we will be inserting categorized lines into.
	private:
		glm::vec3 boundaryEmptyNormal;		// the empty normal applied to any SPolys that are generated as a result of fracturing.
		PolyLogger boxBoundarySPolySetLogger; 
		PolyDebugLevel boxBoundarySPolySetLogLevel = PolyDebugLevel::NONE;
		PolyDebugLevel fracturerDebugLevel = PolyDebugLevel::NONE;
		PolyDebugLevel categorizedLineDebugLevel = PolyDebugLevel::NONE;
		PolyDebugLevel categorizedLineCoplanarTestsDebugLevel = PolyDebugLevel::NONE;
		int currentComparableSPolyIndex = 1;	// this index should always start at 1.
		SPolySupergroup boundarySPolySG;
		bool wasLineProducedByReactor = false;	
		
};

#endif
