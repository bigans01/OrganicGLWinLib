#pragma once

#ifndef MASSZONEBOX_H
#define MASSZONEBOX_H

#include "MassZoneBoxBoundary.h"
#include "MassSubZone.h"
#include "SPolyDOSet.h"
#include "MassZoneBoxType.h"
#include "ECBPPOrientationResults.h"
#include "BlockBorderLineList.h"
#include "EnclaveBorderLineList.h"
#include "ECBBorderLineList.h"
#include "BorderMDFaceList.h"
#include "BorderDataMap.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "ExceptionRecorder.h"

class MassZoneBox
{
	public:
		std::map<BoundaryOrientation, MassZoneBoxBoundary> boxBoundaries;
		void insertNewBoundary(BoundaryOrientation in_BoundaryOrientation, 
							   MassZoneBoxBoundary in_massZoneBoxBoundary, 
			                   SPolyDOSet in_sPolyDOSet,
							   ExceptionRecorder* in_exceptionRecorderRefToPass);
		void printBoundaryLineCounts();
		void printBoundaries();
		void printCategorizedLinesInBoundaries();
		void runSPolyBasedSubZoneAgainstBoundaries(MassSubZone* in_massSubZoneRef);
		std::set<BoundaryOrientation> generateTouchedBoxFacesList(MassZoneBoxType in_massZoneBoxType);
		void runFirstTertiaryProductionPass(std::set<BoundaryOrientation> in_orientationSet, 
											std::map<BoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
		void setContestedCategorizedLineAnalysisFlagInBoundaries();
		void setTouchedGenerationDebugLevel(PolyDebugLevel in_debugLevel);
	private:
		BoundaryOrientation convertPointOrientationToBoundaryOrientation(ECBPPOrientations in_pointOrientation);
		PolyDebugLevel generatedTouchedFaceDebugLevel = PolyDebugLevel::NONE;
};

#endif
