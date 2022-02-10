#pragma once

#ifndef FACERESOLVERBASE_H
#define FACERESOLVERBASE_H

#include "SPoly.h"
#include "InvalidCleaveSequences.h"
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxType.h"
#include "OneDimLineBase.h"
#include "SPolySupergroup.h"
#include <map>
#include <mutex>
#include <iostream>
#include <vector>
#include "CSCorrectionCandidate.h"

class FaceResolverBase
{
	public:
		void initialize(SPoly* in_sPolyPtr,
			InvalidCleaveSequences in_invalids,
			MassZoneBoxType in_boxType,
			MassZoneBoxBoundaryOrientation in_faceOrientation);

		virtual void setupBorderLineRangesAndDimLoc() = 0;	// each derivative of this base class will have 
													// its own method for setting up the line ranges
		virtual void runResolutionAlgorithm() = 0;	// the algorithm that would produce some output of SPolys (resolved, or unresolved);
													// the child class derived from the base class can use its own functions for this.

		virtual void produceMalformedMitigation() = 0;		// the malformed mitigation is produced when the resolver can't actually resolve anything;
															// it produces SPolys and then puts them into the resolution (SPolySupergroup)
		void debugPrintOneDimLines();	// for debug: print the values of the one dim lines
		SPolySupergroup fetchResolution();	// gets a copy of the resulting resolution (to be used by SPolyResolution)
	protected:
		SPoly* sPolyPtr = nullptr;	
		InvalidCleaveSequences invalidsCopy;
		MassZoneBoxType boxType = MassZoneBoxType::NOVAL;
		MassZoneBoxBoundaryOrientation faceOrientation = MassZoneBoxBoundaryOrientation::NONE;
		float dimensionalLimit = 0.0f;		// should always be a 1.0f, 4.0f, or 32.0f, based on whether or not the box type is BLOCK, ENCLAVE, or COLLECTION
		std::map<int, std::unique_ptr<OneDimLineBase>> singleDimLines;	// the "int" int this map will correspond with the ID of each SPoly's border lines
		SPolySupergroup resolution;		// the resolver's produced output (a set of one or more SPolys) after attempting to resolve the CleaveSequences

		void compareCorrectionCandidatesAgainstSequence(std::vector<CSCorrectionCandidate> in_candidates, CleaveSequence* in_invalidPtr);
};

#endif
