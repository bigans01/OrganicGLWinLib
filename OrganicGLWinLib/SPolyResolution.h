#pragma once

#ifndef SPOLYRESOLUTION_H
#define SPOLYRESOLUTION_H

#include "SPoly.h"
#include "SPolyFracturer.h"
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxType.h"
#include "SPolyFracturer.h"
#include "SPolySupergroup.h"
#include "MessageContainer.h"
#include <iostream>
#include <map>
#include "InvalidCleaveSequences.h"
#include "CleaveSequence.h"
#include <mutex>
#include "FaceResolverBase.h"
#include "PosXFaceResolver.h"
#include "NegXFaceResolver.h"
#include "PosYFaceResolver.h"
#include "NegYFaceResolver.h"
#include "PosZFaceResolver.h"
#include "NegZFaceResolver.h"

/*

quick notes:

--the valid CleaveSequences that were already generated will be stored in the referenced SPoly's cleaveMap class member.

*/

class SPolyResolution
{
	public:
		SPolyResolution(SPoly* in_sPolyRef,
						MassZoneBoxBoundaryOrientation in_boundaryOrientation,
						MassZoneBoxType in_zoneBoxType,
						InvalidCleaveSequences in_invalidSequences) :
			resolutionSPolyRef(in_sPolyRef),
			resolutionOrientation(in_boundaryOrientation),
			resolutionBoxType(in_zoneBoxType),
			sequencesToResolve(in_invalidSequences)
		{
			calculateResolution();
			sequencesToResolve.printInvalidSequenceData();	// debug/dev only; remove call at will
		};
		SPolySupergroup fetchResolution();
	private:
		// the following 4 values should be set by the constructor
		SPoly* resolutionSPolyRef = nullptr;	// the SPolyResolution should reasonably assume that the SPoly has made it's call to determineBorderLines
		MassZoneBoxBoundaryOrientation resolutionOrientation = MassZoneBoxBoundaryOrientation::NONE;	
		MassZoneBoxType resolutionBoxType = MassZoneBoxType::NOVAL;
		InvalidCleaveSequences sequencesToResolve;	// contains the invalid CleaveSequences that need to be resolved by this resolution.

		// stores the result of the resolution (whatever it may be) in this super group
		SPolySupergroup calculationResult;

		// resolution related functions
		void calculateResolution();

		// resolver set up
		void determineResolver();

		// unique ptr for the derivative class that is based on FaceResolverBase
		std::unique_ptr<FaceResolverBase> selectedResolverPtr;
};

#endif
