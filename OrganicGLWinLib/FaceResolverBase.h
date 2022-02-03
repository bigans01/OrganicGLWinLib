#pragma once

#ifndef FACERESOLVERBASE_H
#define FACERESOLVERBASE_H

#include "SPoly.h"
#include "InvalidCleaveSequences.h"
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneBoxType.h"
#include "OneDimLine.h"
#include <map>
#include <mutex>
#include <iostream>

class FaceResolverBase
{
	public:
		void initialize(SPoly* in_sPolyPtr, 
						InvalidCleaveSequences in_invalids,
						MassZoneBoxType in_boxType,
						MassZoneBoxBoundaryOrientation in_faceOrientation)
		{
			sPolyPtr = in_sPolyPtr;
			invalidsCopy = in_invalids;
			boxType = in_boxType;
			faceOrientation = in_faceOrientation;

			// the dimensional limit can be determined by simply looking at the box type
			switch (boxType)
			{
				case MassZoneBoxType::BLOCK:
				{ 
					dimensionalLimit = 1.0f; break; 
				}
				case MassZoneBoxType::ENCLAVE:
				{ 
					dimensionalLimit = 4.0f; break; 
				}
				case MassZoneBoxType::COLLECTION: 
				{ 
					dimensionalLimit = 32.0f; break; 
				}
			}
		};

		virtual void setupBorderLineRangesAndDimLoc() = 0;	// each derivative of this base class will have 
													// its own method for setting up the line ranges

	protected:
		SPoly* sPolyPtr = nullptr;	
		InvalidCleaveSequences invalidsCopy;
		MassZoneBoxType boxType = MassZoneBoxType::NOVAL;
		MassZoneBoxBoundaryOrientation faceOrientation = MassZoneBoxBoundaryOrientation::NONE;
		float dimensionalLimit = 0.0f;		// should always be a 1.0f, 4.0f, or 32.0f, based on whether or not the box type is BLOCK, ENCLAVE, or COLLECTION
		std::map<MassZoneBoxBoundaryOrientation, std::unique_ptr<OneDimLine>> singleDimLines;
};

#endif
