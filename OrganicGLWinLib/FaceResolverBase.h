#pragma once

#ifndef FACERESOLVERBASE_H
#define FACERESOLVERBASE_H

#include "SPoly.h"
#include "InvalidCleaveSequences.h"
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxType.h"
#include "OneDimLineBase.h"
#include <map>
#include <mutex>
#include <iostream>

class FaceResolverBase
{
	public:
		void initialize(SPoly* in_sPolyPtr,
			InvalidCleaveSequences in_invalids,
			MassZoneBoxType in_boxType,
			MassZoneBoxBoundaryOrientation in_faceOrientation);

		virtual void setupBorderLineRangesAndDimLoc() = 0;	// each derivative of this base class will have 
													// its own method for setting up the line ranges
		void debugPrintOneDimLines();	// for debug: print the values of the one dim lines
	protected:
		SPoly* sPolyPtr = nullptr;	
		InvalidCleaveSequences invalidsCopy;
		MassZoneBoxType boxType = MassZoneBoxType::NOVAL;
		MassZoneBoxBoundaryOrientation faceOrientation = MassZoneBoxBoundaryOrientation::NONE;
		float dimensionalLimit = 0.0f;		// should always be a 1.0f, 4.0f, or 32.0f, based on whether or not the box type is BLOCK, ENCLAVE, or COLLECTION
		std::map<int, std::unique_ptr<OneDimLineBase>> singleDimLines;	// the "int" int this map will correspond with the ID of each SPoly's border lines
};

#endif
