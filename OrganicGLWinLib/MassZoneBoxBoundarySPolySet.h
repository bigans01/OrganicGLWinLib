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
#include "MassZoneBoxType.h"
#include "SPolyResolution.h"
#include "ExceptionRecorder.h"


class MassZoneBoxBoundarySPolySet
{
	public:
		friend class MassZone;
		friend class MassZoneBoxBoundary;
		friend class MassZoneBox;
		void setBoundarySPolyRef(SPoly* in_sPolyRef);
		void setRecorderRef(ExceptionRecorder* in_exceptionRecorderRef);
		void setLogLevel(PolyDebugLevel in_sPolyDebugLevel);
		void compareSPolySubZoneSPolyToBoundarySPoly(SPoly* in_sPolyRef);
		void insertCategorizedLinesFromNonboundarySPoly(SPoly* in_sPolyRef);
		void setBoundarySPolySG(SPolySupergroup in_boundarySPolySG);	// used to manually set the value of boundarySPolySG; would be used in the case
																		// where a valid typical boundary SPoly already exists and needs to be inserted 
																		// (see the function MassZone::createMassZoneShell, Step 3-B)
		MessageContainer buildBoundarySPolyFromFactory(MassZoneBoxType in_boxTypeValue);	// this is where the "meat and potatoes" of forming the appropriate
																							// results for the boundarySPolySG occurs. The job of this function 
																							// is to produce *some* arrived-at SPoly that this instance of MassZoneBoxBoundarySPolySet
																							// will have in it's boundarySPolySG. A SPolyResolution class must be used,
																							// if:
																							//		1.) There are invalid CleaveSequences, meaning that a SPolyFracturer can't be used.
																							//		2.) The results of an SPolyFracturer run are empty.

																											
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
		bool didCategorizedLineWinContest = false;		// If this value is true, AND there are no generated SPolys for this boundary,
		                                                // an SPoly constituting the entire boundary face will need to be generated.
														// See notes in QMBoolResolveContestedCategorizedLine::solve; 
		ContestableBorderLineMap contestables;					// used only when isContestedCategorizedLineAnalysisEnabled is set to TRUE.
		glm::vec3 boundaryFaceCenterPoint;
		BoundaryOrientation boundarySPolySetOrientation = BoundaryOrientation::NONE;	// needs to be set elsewhere.
		ExceptionRecorder* boundaryRecorderRef = nullptr;

		bool resolveContest(CategorizedLine in_categorizedLine);
};

#endif
