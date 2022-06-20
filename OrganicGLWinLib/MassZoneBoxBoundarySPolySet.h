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
#include "ContestableBorderLineMap.h"
#include "QuatUtils.h"
#include "MessageContainer.h"
#include "BoundaryOrientation.h"
#include "MassZoneBoxType.h"
#include "SPolyResolution.h"


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
		MessageContainer buildBoundarySPolyFromFactory(MassZoneBoxType in_boxTypeValue);
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
		bool isContestedCategorizedLineAnalysisEnabled = false;	// if set to true, "rulings" will be made on contested lines, potentially allowing the construction of an entire boundary SPoly.
		bool requiresContestedAnalysis = false;					
		bool didCategorizedLineWinContest = false;
		ContestableBorderLineMap contestables;					// used only when isContestedCategorizedLineAnalysisEnabled is set to TRUE.
		glm::vec3 boundaryFaceCenterPoint;
		BoundaryOrientation boundarySPolySetOrientation = BoundaryOrientation::NONE;	// needs to be set elsewhere.

		bool resolveContest(CategorizedLine in_categorizedLine);
};

#endif
