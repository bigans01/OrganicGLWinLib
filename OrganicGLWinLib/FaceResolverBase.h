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
		void debugPrintOneDimLines()	// for debug: print the values of the one dim lines
		{
			std::cout << "(FaceResolverBase): printing one dim lines and verifying corresponding border lines..." << std::endl;
			auto singleDimLinesBegin = singleDimLines.begin();
			auto singleDimLinesEnd = singleDimLines.end();
			for (; singleDimLinesBegin != singleDimLinesEnd; singleDimLinesBegin++)
			{
				std::cout << ">   Dim Line at [" << singleDimLinesBegin->first << "]" << std::endl;
				std::cout << "> Metadata: " << std::endl;
				singleDimLinesBegin->second->printMetaData();
				std::cout << "> Corresponding border line: " << std::endl;
				std::cout << "Point A: " << sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.x << ", "
										<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.y << ", "
										<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.z 
					<< "| Point B: " << sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.x << ", "
					<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.y << ", "
					<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.z << std::endl;
			}
		}

	protected:
		SPoly* sPolyPtr = nullptr;	
		InvalidCleaveSequences invalidsCopy;
		MassZoneBoxType boxType = MassZoneBoxType::NOVAL;
		MassZoneBoxBoundaryOrientation faceOrientation = MassZoneBoxBoundaryOrientation::NONE;
		float dimensionalLimit = 0.0f;		// should always be a 1.0f, 4.0f, or 32.0f, based on whether or not the box type is BLOCK, ENCLAVE, or COLLECTION
		std::map<int, std::unique_ptr<OneDimLineBase>> singleDimLines;	// the "int" int this map will correspond with the ID of each SPoly's border lines
};

#endif
