#pragma once

#ifndef SPOLYRESOLUTION_H
#define SPOLYRESOLUTION_H

#include "SPoly.h"
#include "SPolyFracturer.h"
#include "BoundaryOrientation.h"
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
#include "ExceptionRecorder.h"

/*

quick notes:

--the valid CleaveSequences that were already generated will be stored in the referenced SPoly's cleaveMap class member.

--the SPolyResolution class is responsible for passing on data to the chosen FaceResolverBase, so that
  the selected FaceResolverBase instance may appropriately apply the empty normal, BoundaryOrientation values, 
  and SCAB_PARENT/SCAB_CHILD flags to the appropriate SPolys.   

*/

class SPolyResolution
{
	public:
		SPolyResolution(SPoly* in_sPolyRef,
						BoundaryOrientation in_boundaryOrientation,
						MassZoneBoxType in_zoneBoxType,
						InvalidCleaveSequences in_invalidSequences,
						ExceptionRecorder* in_exceptionRecorderRef,
						glm::vec3 in_resolutionMainEmptyNormal) :
			resolutionSPolyRef(in_sPolyRef),
			resolutionOrientation(in_boundaryOrientation),
			resolutionBoxType(in_zoneBoxType),
			sequencesToResolve(in_invalidSequences),
			resolutionRecorderRef(in_exceptionRecorderRef),
			resolutionMainEmptyNormal(in_resolutionMainEmptyNormal)
		{
			calculateResolution();
			sequencesToResolve.printInvalidSequenceData();	// debug/dev only; remove call at will
		};
		SPolySupergroup fetchResolution();
	private:
		// the following 4 values should be set by the constructor
		SPoly* resolutionSPolyRef = nullptr;	// the SPolyResolution should reasonably assume that the SPoly has made it's call to determineBorderLines
		ExceptionRecorder* resolutionRecorderRef = nullptr;
		BoundaryOrientation resolutionOrientation = BoundaryOrientation::NONE;	
		MassZoneBoxType resolutionBoxType = MassZoneBoxType::NOVAL;
		InvalidCleaveSequences sequencesToResolve;	// contains the invalid CleaveSequences that need to be resolved by this resolution.
		glm::vec3 resolutionMainEmptyNormal;	// the main empty normal that represents the face that the resolution is operating on (i.e, 1.0f for POS_X)

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
